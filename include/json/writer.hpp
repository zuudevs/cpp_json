#pragma once

#include "ast.hpp"
#include <string>
#include <cstdio>

namespace json {

class Writer {
public:
    explicit Writer(bool pretty = false, int indent_size = 2)
        : pretty_(pretty), indent_size_(indent_size), indent_(0) {}

    std::string write(const Node* root);
    void write(const Node* root, FILE* out);

private:
    void write_node(const Node* node, std::string& out);
    void write_string(std::string_view str, std::string& out);
    void write_indent(std::string& out);
    void write_newline(std::string& out);

    bool pretty_;
    int indent_size_;
    int indent_;
};

} // namespace json