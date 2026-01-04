#pragma once

#include "error.hpp"
#include <span>
#include <string_view>
#include <cstdint>

namespace json {

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
    explicit Tokenizer(std::span<const char> input)
        : input_(input), pos_(0) {}

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
};

} // namespace json