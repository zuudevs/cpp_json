#pragma once

#include "error.hpp"
#include <span>
#include <string_view>
#include <cstdint>

namespace json {

class Arena; // Forward declaration

enum class TokenType : uint8_t {
    End,
    LeftBrace,      // {
    RightBrace,     // }
    LeftBracket,    // [
    RightBracket,   // ]
    Colon,          // :
    Comma,          // ,
    String,
    Number,
    True,
    False,
    Null
};

struct Token {
    TokenType type;
    std::string_view text;
    size_t offset;
};

class Tokenizer {
public:
    // Constructor without arena (no string unescaping)
    explicit Tokenizer(std::span<const char> input)
        : input_(input), pos_(0), arena_(nullptr) {}
    
    // Constructor with arena (enables string unescaping)
    Tokenizer(std::span<const char> input, Arena& arena)
        : input_(input), pos_(0), arena_(&arena) {}

    Result<Token> next();
    
    [[nodiscard]] size_t position() const { return pos_; }
    [[nodiscard]] bool at_end() const { return pos_ >= input_.size(); }

private:
    void skip_whitespace();
    Result<Token> read_string();
    Result<Token> read_number();
    Result<Token> read_keyword(std::string_view keyword, TokenType type);

    std::span<const char> input_;
    size_t pos_;
    Arena* arena_; // Optional: if null, strings won't be unescaped
};

} // namespace json
