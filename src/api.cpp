#include "json/api.hpp"
#include "json/tokenizer.hpp"
#include "json/parser.hpp"
#include "json/writer.hpp"
#include <fstream>
#include <sstream>
#include <cstring>

namespace json {

Result<Node*> parse(std::span<const char> input, Arena& arena) {
    Tokenizer tokenizer(input, arena); // Pass arena for string unescaping
    Parser parser(arena, tokenizer);
    return parser.parse();
}

Result<Node*> parse(std::string_view input, Arena& arena) {
    return parse(std::span{input.data(), input.size()}, arena);
}

std::string write(const Node* root, bool pretty) {
    Writer writer(pretty);
    return writer.write(root);
}

Result<Node*> read_file_to_arena(const char* filename, Arena& arena) {
    // Open file
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        return std::unexpected(Error{
            ErrorCode::OutOfMemory,  // Reusing this for I/O errors
            0,
            "Cannot open file"
        });
    }

    // Get file size
    std::streamsize size = file.tellg();
    if (size < 0) {
        return std::unexpected(Error{
            ErrorCode::OutOfMemory,
            0,
            "Cannot determine file size"
        });
    }
    file.seekg(0, std::ios::beg);

    // Allocate buffer in arena
    char* buffer = arena.alloc<char>(static_cast<size_t>(size));
    if (!buffer) {
        return std::unexpected(Error{
            ErrorCode::OutOfMemory,
            0,
            "Arena allocation failed"
        });
    }

    // Read file into buffer
    if (!file.read(buffer, size)) {
        return std::unexpected(Error{
            ErrorCode::OutOfMemory,
            0,
            "Failed to read file"
        });
    }

    // Parse the buffer
    return parse(std::span{buffer, static_cast<size_t>(size)}, arena);
}

Result<Node*> read_file_to_arena(std::string_view filename, Arena& arena) {
    // Create null-terminated string for fstream
    std::string filename_str(filename);
    return read_file_to_arena(filename_str.c_str(), arena);
}

} // namespace json
