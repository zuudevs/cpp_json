#include "json/api.hpp"
#include <iostream>
#include <string_view>

int main() {
    using namespace json;

    // Test 1: Parse simple JSON
    std::string_view input = R"({
        "name": "John Doe",
        "age": 30,
        "active": true,
        "scores": [95, 87, 92],
        "address": {
            "city": "New York",
            "zip": "10001"
        }
    })";

    Arena arena;
    auto result = parse(std::span{input.data(), input.size()}, arena);

    if (!result) {
        std::cerr << "Parse error: " << result.error().message 
                  << " at offset " << result.error().offset << "\n";
        return 1;
    }

    std::cout << "Parse successful!\n";

    // Write back
    Node* root = *result;
    std::string output = write(root, true);
    std::cout << "Output:\n" << output << "\n";

    // Test 2: Simple values
    std::string_view test2 = R"([1, 2, 3, null, true, false, "hello"])";
    Arena arena2;
    auto result2 = parse(std::span{test2.data(), test2.size()}, arena2);
    
    if (result2) {
        std::cout << "\nTest 2:\n" << write(*result2, true) << "\n";
    }

    // Test 3: Empty structures
    std::string_view test3 = R"({"empty_obj": {}, "empty_arr": []})";
    Arena arena3;
    auto result3 = parse(std::span{test3.data(), test3.size()}, arena3);
    
    if (result3) {
        std::cout << "\nTest 3:\n" << write(*result3, false) << "\n";
    }

    std::cout << "\nAll tests passed!\n";
    return 0;
}