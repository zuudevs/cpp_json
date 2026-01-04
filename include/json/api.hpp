/**
 * @file api.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief High-level JSON API
 * 
 * This file provides the main entry points for parsing and writing JSON data.
 * It includes convenience functions for file I/O and string processing.
 * @version 1.0.1
 * @date 2026-01-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "ast.hpp"
#include "arena.hpp"
#include "error.hpp"
#include <span>
#include <string>
#include <string_view>

namespace json {

/**
 * @brief Parses a JSON string from a character span.
 * 
 * @param input The input buffer containing JSON data.
 * @param arena The memory arena used for allocating AST nodes.
 * @return Result<Node*> The root node of the parsed AST, or an error.
 */
Result<Node*> parse(std::span<const char> input, Arena& arena);

/**
 * @brief Parses a JSON string from a string view.
 * 
 * @param input The input string containing JSON data.
 * @param arena The memory arena used for allocating AST nodes.
 * @return Result<Node*> The root node of the parsed AST, or an error.
 */
Result<Node*> parse(std::string_view input, Arena& arena);

/**
 * @brief Serializes an AST into a JSON string.
 * 
 * @param root The root node of the AST to serialize.
 * @param pretty If true, formats the output with indentation and newlines.
 * @return std::string The resulting JSON string.
 */
std::string write(const Node* root, bool pretty = false);

/**
 * @brief Read a file into arena-managed memory and parse it
 * 
 * This function:
 * 1. Reads the entire file into a buffer
 * 2. Allocates the buffer in the provided arena
 * 3. Parses the JSON content
 * 
 * @param filename Path to the JSON file
 * @param arena Arena allocator for both the buffer and parsed nodes
 * @return Result<Node*> The root node of the parsed AST, or an error.
 * 
 * Example:
 *   Arena arena;
 *   auto result = read_file_to_arena("data.json", arena);
 *   if (result) {
 *       Node* root = *result;
 *       // Use root...
 *   }
 */
Result<Node*> read_file_to_arena(const char* filename, Arena& arena);

/**
 * @brief Read a file into arena-managed memory and parse it (overload).
 * 
 * @param filename Path to the JSON file as a string view.
 * @param arena Arena allocator for both the buffer and parsed nodes.
 * @return Result<Node*> The root node of the parsed AST, or an error.
 */
Result<Node*> read_file_to_arena(std::string_view filename, Arena& arena);

/**
 * @brief Parse JSON from file (convenience alias).
 * 
 * Equivalent to read_file_to_arena but with a more explicit naming convention.
 * 
 * @param filename Path to the JSON file.
 * @param arena Arena allocator for both the buffer and parsed nodes.
 * @return Result<Node*> The root node of the parsed AST, or an error.
 */
inline Result<Node*> parse_file(const char* filename, Arena& arena) {
    return read_file_to_arena(filename, arena);
}

/**
 * @brief Parse JSON from file (convenience alias overload).
 * 
 * @param filename Path to the JSON file as a string view.
 * @param arena Arena allocator for both the buffer and parsed nodes.
 * @return Result<Node*> The root node of the parsed AST, or an error.
 */
inline Result<Node*> parse_file(std::string_view filename, Arena& arena) {
    return read_file_to_arena(filename, arena);
}

} // namespace json
