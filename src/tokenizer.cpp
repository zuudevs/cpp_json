#include "json/tokenizer.hpp"
#include "json/arena.hpp"
#include <cctype>
#include <cstring>

namespace json {

// Helper to convert hex char to int
constexpr int hex_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

// Helper to decode \uXXXX escape sequence
constexpr int decode_unicode_escape(const char* ptr) {
    int val = 0;
    for (int i = 0; i < 4; ++i) {
        int h = hex_to_int(ptr[i]);
        if (h < 0) return -1;
        val = (val << 4) | h;
    }
    return val;
}

// Helper to encode UTF-8 from codepoint
constexpr size_t encode_utf8(int codepoint, char* out) {
    if (codepoint < 0x80) {
        out[0] = static_cast<char>(codepoint);
        return 1;
    } else if (codepoint < 0x800) {
        out[0] = static_cast<char>(0xC0 | (codepoint >> 6));
        out[1] = static_cast<char>(0x80 | (codepoint & 0x3F));
        return 2;
    } else if (codepoint < 0x10000) {
        out[0] = static_cast<char>(0xE0 | (codepoint >> 12));
        out[1] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        out[2] = static_cast<char>(0x80 | (codepoint & 0x3F));
        return 3;
    } else if (codepoint < 0x110000) {
        out[0] = static_cast<char>(0xF0 | (codepoint >> 18));
        out[1] = static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        out[2] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        out[3] = static_cast<char>(0x80 | (codepoint & 0x3F));
        return 4;
    }
    return 0;
}

void Tokenizer::skip_whitespace() {
    while (pos_ < input_.size()) {
        char c = input_[pos_];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            ++pos_;
        } else {
            break;
        }
    }
}

Result<Token> Tokenizer::next() {
    skip_whitespace();

    if (at_end()) {
        return Token{TokenType::End, "", pos_};
    }

    size_t start = pos_;
    char c = input_[pos_];

    switch (c) {
        case '{': ++pos_; return Token{TokenType::LeftBrace, "{", start};
        case '}': ++pos_; return Token{TokenType::RightBrace, "}", start};
        case '[': ++pos_; return Token{TokenType::LeftBracket, "[", start};
        case ']': ++pos_; return Token{TokenType::RightBracket, "]", start};
        case ':': ++pos_; return Token{TokenType::Colon, ":", start};
        case ',': ++pos_; return Token{TokenType::Comma, ",", start};
        case '"': return read_string();
        case 't': return read_keyword("true", TokenType::True);
        case 'f': return read_keyword("false", TokenType::False);
        case 'n': return read_keyword("null", TokenType::Null);
        case '-':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return read_number();
        default:
            return std::unexpected(Error{ErrorCode::InvalidToken, pos_, 
                                       error_message(ErrorCode::InvalidToken)});
    }
}

Result<Token> Tokenizer::read_string() {
    size_t start = pos_;
    ++pos_; // Skip opening quote

    // First pass: check if there are any escape sequences
    bool has_escapes = false;
    size_t scan_pos = pos_;
    
    while (scan_pos < input_.size()) {
        char c = input_[scan_pos];
        
        if (c == '"') {
            break;
        }
        
        if (c == '\\') {
            has_escapes = true;
            ++scan_pos;
            if (scan_pos >= input_.size()) {
                return std::unexpected(Error{ErrorCode::InvalidEscape, scan_pos,
                                           error_message(ErrorCode::InvalidEscape)});
            }
            ++scan_pos;
        } else {
            ++scan_pos;
        }
    }

    if (scan_pos >= input_.size()) {
        return std::unexpected(Error{ErrorCode::InvalidString, start,
                                   error_message(ErrorCode::InvalidString)});
    }

    // Fast path: no escapes, zero-copy string
    if (!has_escapes) {
        size_t content_size = scan_pos - pos_;
        std::string_view text(input_.data() + start, scan_pos + 1 - start);
        pos_ = scan_pos + 1; // Skip closing quote
        return Token{TokenType::String, text, start};
    }

    // Slow path: process escapes and allocate new buffer
    // Allocate maximum needed size (worst case: no escapes reduce size)
    size_t content_size = scan_pos - pos_;
    char* buffer = arena_->alloc<char>(content_size * 4 + 3); // Max UTF-8 expansion + quotes + null
    char* write_ptr = buffer;
    
    // Add opening quote to buffer
    *write_ptr++ = '"';
    
    size_t read_pos = pos_;
    while (read_pos < scan_pos) {
        char c = input_[read_pos];
        
        if (c == '\\') {
            ++read_pos;
            if (read_pos >= scan_pos) {
                return std::unexpected(Error{ErrorCode::InvalidEscape, read_pos,
                                           error_message(ErrorCode::InvalidEscape)});
            }
            
            char escape = input_[read_pos];
            switch (escape) {
                case '"':  *write_ptr++ = '"'; break;
                case '\\': *write_ptr++ = '\\'; break;
                case '/':  *write_ptr++ = '/'; break;
                case 'b':  *write_ptr++ = '\b'; break;
                case 'f':  *write_ptr++ = '\f'; break;
                case 'n':  *write_ptr++ = '\n'; break;
                case 'r':  *write_ptr++ = '\r'; break;
                case 't':  *write_ptr++ = '\t'; break;
                case 'u': {
                    // Unicode escape: \uXXXX
                    ++read_pos;
                    if (read_pos + 3 >= input_.size()) {
                        return std::unexpected(Error{ErrorCode::InvalidEscape, read_pos,
                                                   error_message(ErrorCode::InvalidEscape)});
                    }
                    
                    int codepoint = decode_unicode_escape(input_.data() + read_pos);
                    if (codepoint < 0) {
                        return std::unexpected(Error{ErrorCode::InvalidEscape, read_pos,
                                                   "Invalid unicode escape sequence"});
                    }
                    
                    // Handle UTF-16 surrogate pairs
                    if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                        // High surrogate, need low surrogate
                        read_pos += 4;
                        if (read_pos + 5 >= input_.size() || 
                            input_[read_pos] != '\\' || input_[read_pos + 1] != 'u') {
                            return std::unexpected(Error{ErrorCode::InvalidEscape, read_pos,
                                                       "Missing low surrogate"});
                        }
                        read_pos += 2;
                        int low = decode_unicode_escape(input_.data() + read_pos);
                        if (low < 0xDC00 || low > 0xDFFF) {
                            return std::unexpected(Error{ErrorCode::InvalidEscape, read_pos,
                                                       "Invalid low surrogate"});
                        }
                        codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
                        read_pos += 3; // Will be incremented at end
                    } else {
                        read_pos += 3; // Will be incremented at end
                    }
                    
                    size_t utf8_len = encode_utf8(codepoint, write_ptr);
                    if (utf8_len == 0) {
                        return std::unexpected(Error{ErrorCode::InvalidEscape, read_pos,
                                                   "Invalid codepoint"});
                    }
                    write_ptr += utf8_len;
                    break;
                }
                default:
                    return std::unexpected(Error{ErrorCode::InvalidEscape, read_pos,
                                               "Unknown escape sequence"});
            }
            ++read_pos;
        } else {
            *write_ptr++ = c;
            ++read_pos;
        }
    }
    
    // Add closing quote
    *write_ptr++ = '"';
    *write_ptr = '\0';
    
    pos_ = scan_pos + 1; // Skip closing quote in input
    
    std::string_view processed_text(buffer, write_ptr - buffer);
    return Token{TokenType::String, processed_text, start};
}

Result<Token> Tokenizer::read_number() {
    size_t start = pos_;

    if (input_[pos_] == '-') ++pos_;

    if (pos_ >= input_.size() || !std::isdigit(input_[pos_])) {
        return std::unexpected(Error{ErrorCode::InvalidNumber, start,
                                   error_message(ErrorCode::InvalidNumber)});
    }

    // Integer part
    if (input_[pos_] == '0') {
        ++pos_;
    } else {
        while (pos_ < input_.size() && std::isdigit(input_[pos_])) {
            ++pos_;
        }
    }

    // Fractional part
    if (pos_ < input_.size() && input_[pos_] == '.') {
        ++pos_;
        if (pos_ >= input_.size() || !std::isdigit(input_[pos_])) {
            return std::unexpected(Error{ErrorCode::InvalidNumber, start,
                                       error_message(ErrorCode::InvalidNumber)});
        }
        while (pos_ < input_.size() && std::isdigit(input_[pos_])) {
            ++pos_;
        }
    }

    // Exponent part
    if (pos_ < input_.size() && (input_[pos_] == 'e' || input_[pos_] == 'E')) {
        ++pos_;
        if (pos_ < input_.size() && (input_[pos_] == '+' || input_[pos_] == '-')) {
            ++pos_;
        }
        if (pos_ >= input_.size() || !std::isdigit(input_[pos_])) {
            return std::unexpected(Error{ErrorCode::InvalidNumber, start,
                                       error_message(ErrorCode::InvalidNumber)});
        }
        while (pos_ < input_.size() && std::isdigit(input_[pos_])) {
            ++pos_;
        }
    }

    std::string_view text(input_.data() + start, pos_ - start);
    return Token{TokenType::Number, text, start};
}

Result<Token> Tokenizer::read_keyword(std::string_view keyword, TokenType type) {
    size_t start = pos_;
    
    for (size_t i = 0; i < keyword.size(); ++i) {
        if (pos_ >= input_.size() || input_[pos_] != keyword[i]) {
            return std::unexpected(Error{ErrorCode::InvalidToken, start,
                                       error_message(ErrorCode::InvalidToken)});
        }
        ++pos_;
    }

    std::string_view text(input_.data() + start, keyword.size());
    return Token{type, text, start};
}

} // namespace json
