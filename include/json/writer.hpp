/**
 * @file writer.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief JSON Writer API
 * 
 * This file defines the high-level API for writing JSON from an AST.
 * @version 1.0.0
 * @date 2026-01-04
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "ast.hpp"
#include <string>
#include <string_view>
#include <cstdio>

namespace json {

/**
 * @brief A class for serializing JSON AST nodes into text.
 * 
 * The Writer class provides functionality to convert a hierarchy of Node objects
 * into a JSON string or write it directly to a file stream. It supports
 * optional pretty-printing with configurable indentation.
 */
class Writer {
public:
    /**
     * @brief Constructs a new Writer object.
     * 
     * @param pretty If true, enables pretty-printing (newlines and indentation). Defaults to false.
     * @param indent_size The number of spaces used for indentation when pretty-printing is enabled. Defaults to 2.
     */
    explicit Writer(bool pretty = false, int indent_size = 2)
        : pretty_(pretty)
        , indent_size_(indent_size)
        , indent_(0) {}

    /**
     * @brief Serializes the AST starting from the given root node into a string.
     * 
     * @param root A pointer to the root Node of the AST to serialize.
     * @return std::string The JSON string representation of the AST.
     */
    std::string write(const Node* root);

    /**
     * @brief Serializes the AST starting from the given root node to a file stream.
     * 
     * @param root A pointer to the root Node of the AST to serialize.
     * @param out The file stream to write to (e.g., stdout or a file handle).
     */
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
