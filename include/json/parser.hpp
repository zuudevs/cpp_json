/**
 * @file parser.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief JSON Parser API
 * 
 * This file defines the Parser class responsible for constructing an AST
 * from a stream of tokens.
 * @version 1.0.0
 * @date 2026-01-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "ast.hpp"
#include "arena.hpp"
#include "tokenizer.hpp"
#include "error.hpp"

namespace json {

/**
 * @brief Recursive descent parser for JSON.
 * 
 * The Parser class consumes tokens from a Tokenizer and constructs a
 * hierarchical Abstract Syntax Tree (AST) stored in the provided Arena.
 * It enforces strict JSON syntax and handles recursion depth limits.
 */
class Parser {
public:
    /**
     * @brief Constructs a new Parser object.
     * 
     * @param arena The memory arena used for allocating AST nodes.
     * @param tokenizer The source of JSON tokens.
     */
    Parser(Arena& arena, Tokenizer& tokenizer)
        : arena_(arena), tok_(tokenizer), depth_(0) {}

    /**
     * @brief Parses the entire JSON input into an AST.
     * 
     * This method triggers the parsing process. It expects a single valid
     * JSON value (object, array, string, number, bool, or null) at the root.
     * 
     * @return Result<Node*> The root node of the generated AST, or an error if parsing fails.
     */
    Result<Node*> parse();

private:
    /// Maximum allowed recursion depth to prevent stack overflow.
    static constexpr size_t MaxDepth = 256;

    Result<Node*> parse_value();
    Result<Node*> parse_object();
    Result<Node*> parse_array();
    
    Result<Token> expect(TokenType type);
    Result<Token> peek();
    void consume();

    Arena& arena_;
    Tokenizer& tok_;
    Token current_;
    bool has_current_ = false;
    size_t depth_;
};

} // namespace json
