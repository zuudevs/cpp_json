#include "json/api.hpp"
#include "json/builder.hpp"
#include <iostream>
#include <fstream>
#include <format>

using namespace json;

static void example_1_safe_accessors() {
    std::cout << "=== Example 1: Safe Accessors ===" << "\n\n";
    
    std::string_view input = R"({
        "name": "Alice",
        "age": 30,
        "active": true,
        "scores": [95, 87, 92]
    })";

    Arena arena;
    auto result = parse(input, arena);
    if (!result) {
        std::cerr << "Parse error: " << result.error().message << "\n";
        return;
    }

    Node* root = *result;

    // Type-safe access with std::expected
    if (auto name = (*root)["name"]) {
        if (auto str = (*name)->as_string()) {
            std::cout << std::format("Name: {}\n", *str);
        }
    }

    if (auto age = (*root)["age"]) {
        if (auto num = (*age)->as_number()) {
            std::cout << std::format("Age: {}\n", *num);
        }
    }

    // Handle errors gracefully
    auto missing = (*root)["nonexistent"];
    if (!missing) {
        std::cout << std::format("Error: {}\n", missing.error().message);
    }

    std::cout << "\n";
}

static void example_2_iterators() {
    std::cout << "=== Example 2: Modern C++ Iterators ===" << "\n\n";
    
    std::string_view input = R"({
        "colors": ["red", "green", "blue"],
        "settings": {
            "theme": "dark",
            "fontSize": 14,
            "autoSave": true
        }
    })";

    Arena arena;
    auto result = parse(input, arena);
    if (!result) return;

    Node* root = *result;

    // Range-based for loop on arrays
    if (auto colors = (*root)["colors"]) {
        if (auto arr = (*colors)->as_array()) {
            std::cout << "Colors:\n";
            for (Node* color : *arr) {
                if (auto str = color->as_string()) {
                    std::cout << std::format("  - {}\n", *str);
                }
            }
        }
    }

    // Structured bindings on objects
    if (auto settings = (*root)["settings"]) {
        if (auto obj = (*settings)->as_object()) {
            std::cout << "\nSettings:\n";
            for (const auto& [key, value] : *obj) {
                std::cout << std::format("  {}: ", key.view());
                
                if (auto str = value->as_string()) {
                    std::cout << std::format("\"{}\"\n", *str);
                } else if (auto num = value->as_number()) {
                    std::cout << std::format("{}\n", *num);
                } else if (auto b = value->as_bool()) {
                    std::cout << std::format("{}\n", *b ? "true" : "false");
                }
            }
        }
    }

    std::cout << "\n";
}

static void example_3_builder_api() {
    std::cout << "=== Example 3: Safe Builder API ===" << "\n\n";
    
    Arena arena;

    // Build a complex object using fluent API
    auto builder = build_object(arena);
    builder.add("user", "Bob")
           .add("id", 12345)
           .add("verified", true)
           .add("score", 98.5);

    // Build nested arrays
    auto tags = build_array(arena);
    tags.add("developer").add("admin").add("moderator");

    builder.add("tags", tags.build());

    // Build nested objects
    auto address = build_object(arena);
    address.add("street", "123 Main St")
           .add("city", "New York")
           .add("zip", "10001");

    builder.add("address", address.build());

    Node* root = builder.build();

    // Output the constructed JSON
    std::cout << write(root, true) << "\n\n";
}

static void example_4_escape_sequences() {
    std::cout << "=== Example 4: String Unescaping ===" << "\n\n";
    
    std::string_view input = R"({
        "message": "Hello\nWorld",
        "path": "C:\\Users\\Documents",
        "quote": "She said \"Hi\"",
        "unicode": "\u0048\u0065\u006C\u006C\u006F",
        "emoji": "\uD83D\uDE00"
    })";

    Arena arena;
    auto result = parse(input, arena);
    if (!result) {
        std::cerr << "Parse error\n";
        return;
    }

    Node* root = *result;
    if (auto obj = root->as_object()) {
        for (const auto& [key, value] : *obj) {
            if (auto str = value->as_string()) {
                std::cout << std::format("{}: \"{}\"\n", key.view(), *str);
            }
        }
    }

    std::cout << "\n";
}

static void example_5_file_reading() {
    std::cout << "=== Example 5: File Reading ===" << "\n\n";
    
    // Create a test file
    {
        std::ofstream file("test.json");
        file << R"({
            "project": "cpp_json",
            "version": "2.0",
            "features": ["fast", "safe", "modern"]
        })";
    }

    // Read and parse file directly
    Arena arena;
    auto result = parse_file("test.json", arena);
    
    if (!result) {
        std::cerr << "Failed to read file: " << result.error().message << "\n";
        return;
    }

    Node* root = *result;
    std::cout << "Loaded from file:\n" << write(root, true) << "\n\n";
}

static void example_6_error_handling() {
    std::cout << "=== Example 6: Error Handling ===" << "\n\n";
    
    std::string_view input = R"({"array": [1, 2, 3]})";
    
    Arena arena;
    auto result = parse(input, arena);
    if (!result) return;

    Node* root = *result;

    // Array bounds checking
    if (auto array_result = (*root)["array"]) {
        Node* array = *array_result;
        
        // Valid access
        if (auto elem = (*array)[1]) {
            Node* element = *elem;
            std::cout << std::format("Element [1]: {}\n", 
                element->as_number().value_or(0.0));
        }

        // Out of bounds
        auto invalid = (*array)[10];
        if (!invalid) {
            std::cout << std::format("Out of bounds: {}\n", 
                invalid.error().message);
        }
    }

    // Type mismatch
    if (auto array_result = (*root)["array"]) {
        Node* array = *array_result;
        auto wrong_type = array->as_string();
        if (!wrong_type) {
            std::cout << std::format("Type error: {}\n", 
                wrong_type.error().message);
        }
    }

    std::cout << "\n";
}

int main() {
    example_1_safe_accessors();
    example_2_iterators();
    example_3_builder_api();
    example_4_escape_sequences();
    example_5_file_reading();
    example_6_error_handling();

    std::cout << "All examples completed successfully!\n";
    return 0;
}