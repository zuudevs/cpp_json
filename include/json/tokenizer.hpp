/**
 * @file tokenizer.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief JSON Tokenizer API
 * 
 * This file defines the Tokenizer class responsible for lexing JSON input
 * into a stream of tokens.
 * @version 1.1.2
 * @date 2026-01-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "error.hpp"
#include <span>
#include <string_view>
#include <cstdint>

namespace json {

class Arena; // Forward declaration

/**
 * @brief Represents the type of a JSON token.
 */
enum class TokenType : uint8_t {
    End,            ///< End of input
    LeftBrace,      ///< '{' - Start of object
    RightBrace,     ///< '}' - End of object
    LeftBracket,    ///< '[' - Start of array
    RightBracket,   ///< ']' - End of array
    Colon,          ///< ':' - Key-value separator
    Comma,          ///< ',' - Element separator
    String,         ///< String literal
    Number,         ///< Number literal
    True,           ///< 'true' keyword
    False,          ///< 'false' keyword
    Null            ///< 'null' keyword
};

/**
 * @brief Represents a single token in the JSON stream.
 */
struct Token {
    TokenType type;         ///< The type of the token
    std::string_view text;  ///< The text content of the token
    size_t offset;          ///< The byte offset of the token in the input
};

/**
 * @brief Lexer for JSON input.
 * 
 * The Tokenizer breaks down a raw character buffer into a sequence of Tokens.
 * It handles whitespace skipping and basic validation of token formats.
 */
class Tokenizer {
public:
    /**
     * @brief Constructs a Tokenizer without an arena.
     * 
     * @param input The input character buffer to tokenize.
     */
    explicit Tokenizer(std::span<const char> input)
        : input_(input), pos_(0), arena_(nullptr) {}
    
    /**
     * @brief Constructs a Tokenizer with a memory arena.
     * 
     * Providing an arena allows the tokenizer to allocate memory for
     * processed strings (e.g., unescaping special characters).
     * 
     * @param input The input character buffer to tokenize.
     * @param arena The memory arena for allocations.
     */
    Tokenizer(std::span<const char> input, Arena& arena)
        : input_(input), pos_(0), arena_(&arena) {}

    /**
     * @brief Retrieves the next token from the input.
     * 
     * @return Result<Token> The next token found, or an error if the input is invalid.
     */
    Result<Token> next();
    
    /**
     * @brief Gets the current position in the input buffer.
     * 
     * @return size_t The current byte offset.
     */
    [[nodiscard]] size_t position() const { return pos_; }

    /**
     * @brief Checks if the tokenizer has reached the end of the input.
     * 
     * @return true If the end of input has been reached.
     * @return false Otherwise.
     */
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
