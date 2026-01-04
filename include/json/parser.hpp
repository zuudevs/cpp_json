#pragma once

#include "ast.hpp"
#include "arena.hpp"
#include "tokenizer.hpp"
#include "error.hpp"

namespace json {

class Parser {
public:
    Parser(Arena& arena, Tokenizer& tokenizer)
        : arena_(arena), tok_(tokenizer), depth_(0) {}

    Result<Node*> parse();

private:
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