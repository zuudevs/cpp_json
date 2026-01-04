#include "json/parser.hpp"
#include <vector>
#include <charconv>

namespace json {

Result<Node*> Parser::parse() {
    auto result = parse_value();
    if (!result) return result;

    // Verify we're at end of input
    auto token = peek();
    if (!token) return std::unexpected(token.error());
    
    if (token->type != TokenType::End) {
        return std::unexpected(Error{ErrorCode::UnexpectedToken, token->offset,
                                   error_message(ErrorCode::UnexpectedToken)});
    }

    return result;
}

Result<Token> Parser::peek() {
    if (!has_current_) {
        auto token = tok_.next();
        if (!token) return token;
        current_ = *token;
        has_current_ = true;
    }
    return current_;
}

void Parser::consume() {
    has_current_ = false;
}

Result<Token> Parser::expect(TokenType type) {
    auto token = peek();
    if (!token) return token;
    
    if (token->type != type) {
        return std::unexpected(Error{ErrorCode::UnexpectedToken, token->offset,
                                   error_message(ErrorCode::UnexpectedToken)});
    }
    
    consume();
    return token;
}

Result<Node*> Parser::parse_value() {
    auto token = peek();
    if (!token) return std::unexpected(token.error());

    switch (token->type) {
        case TokenType::Null: {
            consume();
            Node* node = arena_.alloc<Node>();
            *node = Node::make_null();
            return node;
        }
            
        case TokenType::True:
        case TokenType::False: {
            bool val = token->type == TokenType::True;
            consume();
            Node* node = arena_.alloc<Node>();
            *node = Node::make_bool(val);
            return node;
        }
            
        case TokenType::Number: {
            double val;
            auto result = std::from_chars(token->text.data(), 
                                         token->text.data() + token->text.size(), 
                                         val);
            if (result.ec != std::errc{}) {
                return std::unexpected(Error{ErrorCode::InvalidNumber, token->offset,
                                           error_message(ErrorCode::InvalidNumber)});
            }
            consume();
            Node* node = arena_.alloc<Node>();
            *node = Node::make_number(val);
            return node;
        }
            
        case TokenType::String: {
            // String content has already been unescaped by tokenizer
            // Extract string content (remove quotes)
            std::string_view text = token->text;
            if (text.size() >= 2) {
                text = text.substr(1, text.size() - 2);
            }
            consume();
            Node* node = arena_.alloc<Node>();
            *node = Node::make_string(text.data(), text.size());
            return node;
        }
            
        case TokenType::LeftBracket:
            return parse_array();
            
        case TokenType::LeftBrace:
            return parse_object();
            
        default:
            return std::unexpected(Error{ErrorCode::ExpectedValue, token->offset,
                                       error_message(ErrorCode::ExpectedValue)});
    }
}

Result<Node*> Parser::parse_array() {
    if (++depth_ > MaxDepth) {
        return std::unexpected(Error{ErrorCode::TooDeep, tok_.position(),
                                   error_message(ErrorCode::TooDeep)});
    }

    auto left = expect(TokenType::LeftBracket);
    if (!left) return std::unexpected(left.error());

    std::vector<Node*> elements;

    auto token = peek();
    if (!token) return std::unexpected(token.error());

    if (token->type == TokenType::RightBracket) {
        consume();
        --depth_;
        Node* node = arena_.alloc<Node>();
        *node = Node::make_array(nullptr, 0);
        return node;
    }

    while (true) {
        auto element = parse_value();
        if (!element) return element;
        elements.push_back(*element);

        token = peek();
        if (!token) return std::unexpected(token.error());

        if (token->type == TokenType::RightBracket) {
            consume();
            break;
        }

        if (token->type != TokenType::Comma) {
            return std::unexpected(Error{ErrorCode::ExpectedComma, token->offset,
                                       error_message(ErrorCode::ExpectedComma)});
        }
        consume();
    }

    --depth_;

    // Copy to arena
    Node** arr = arena_.alloc<Node*>(elements.size());
    for (size_t i = 0; i < elements.size(); ++i) {
        arr[i] = elements[i];
    }

    Node* node = arena_.alloc<Node>();
    *node = Node::make_array(arr, elements.size());
    return node;
}

Result<Node*> Parser::parse_object() {
    if (++depth_ > MaxDepth) {
        return std::unexpected(Error{ErrorCode::TooDeep, tok_.position(),
                                   error_message(ErrorCode::TooDeep)});
    }

    auto left = expect(TokenType::LeftBrace);
    if (!left) return std::unexpected(left.error());

    std::vector<ObjectPair> pairs;

    auto token = peek();
    if (!token) return std::unexpected(token.error());

    if (token->type == TokenType::RightBrace) {
        consume();
        --depth_;
        Node* node = arena_.alloc<Node>();
        *node = Node::make_object(nullptr, 0);
        return node;
    }

    while (true) {
        auto key = expect(TokenType::String);
        if (!key) return std::unexpected(key.error());

        // String content has already been unescaped by tokenizer
        // Extract string content (remove quotes)
        std::string_view key_text = key->text;
        if (key_text.size() >= 2) {
            key_text = key_text.substr(1, key_text.size() - 2);
        }

        auto colon = expect(TokenType::Colon);
        if (!colon) return std::unexpected(colon.error());

        auto value = parse_value();
        if (!value) return value;

        pairs.push_back(ObjectPair{{key_text.data(), key_text.size()}, *value});

        token = peek();
        if (!token) return std::unexpected(token.error());

        if (token->type == TokenType::RightBrace) {
            consume();
            break;
        }

        if (token->type != TokenType::Comma) {
            return std::unexpected(Error{ErrorCode::ExpectedComma, token->offset,
                                       error_message(ErrorCode::ExpectedComma)});
        }
        consume();
    }

    --depth_;

    // Copy to arena
    ObjectPair* obj = arena_.alloc<ObjectPair>(pairs.size());
    for (size_t i = 0; i < pairs.size(); ++i) {
        obj[i] = pairs[i];
    }

    Node* node = arena_.alloc<Node>();
    *node = Node::make_object(obj, pairs.size());
    return node;
}

} // namespace json
