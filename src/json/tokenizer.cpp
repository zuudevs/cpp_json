#include "json/tokenizer.hpp"
#include <cctype>

namespace json {

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

    while (pos_ < input_.size()) {
        char c = input_[pos_];
        
        if (c == '"') {
            ++pos_;
            std::string_view text(input_.data() + start, pos_ - start);
            return Token{TokenType::String, text, start};
        }
        
        if (c == '\\') {
            ++pos_;
            if (pos_ >= input_.size()) {
                return std::unexpected(Error{ErrorCode::InvalidEscape, pos_,
                                           error_message(ErrorCode::InvalidEscape)});
            }
            // Skip escaped character
            ++pos_;
        } else {
            ++pos_;
        }
    }

    return std::unexpected(Error{ErrorCode::InvalidString, start,
                               error_message(ErrorCode::InvalidString)});
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