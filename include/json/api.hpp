#pragma once

#include "ast.hpp"
#include "arena.hpp"
#include "error.hpp"
#include <span>
#include <string>
#include <string_view>

namespace json {

// High-level parsing API
Result<Node*> parse(std::span<const char> input, Arena& arena);
Result<Node*> parse(std::string_view input, Arena& arena);

// High-level writing API
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
 * @return Parsed JSON node or error
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
Result<Node*> read_file_to_arena(std::string_view filename, Arena& arena);

/**
 * @brief Parse JSON from file (convenience function)
 * 
 * Equivalent to read_file_to_arena but more explicit naming
 */
inline Result<Node*> parse_file(const char* filename, Arena& arena) {
    return read_file_to_arena(filename, arena);
}

inline Result<Node*> parse_file(std::string_view filename, Arena& arena) {
    return read_file_to_arena(filename, arena);
}

} // namespace json
