#include "json/api.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <format>

void print_usage(const char* prog) {
    std::cerr << std::format("Usage: {} <command> [options] <file>\n", prog);
    std::cerr << "Commands:\n";
    std::cerr << "  validate   Check if JSON is valid\n";
    std::cerr << "  format     Format JSON (pretty print)\n";
    std::cerr << "  minify     Compact JSON (remove whitespace)\n";
    std::cerr << "  stats      Show JSON statistics\n";
}

std::string read_input(const std::string& path) {
    if (path == "-") {
        std::stringstream ss;
        ss << std::cin.rdbuf();
        return ss.str();
    }
    std::ifstream f(path, std::ios::binary);
    if (!f) throw std::runtime_error("Cannot open file");
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

void print_stats(const json::Node* node) {
    // Simple recursive stats counter
    struct Stats { size_t obj=0, arr=0, str=0, num=0, bool_val=0, null_val=0, depth=0; } stats;
    auto traverse = [&](auto&& self, const json::Node* n, size_t d) -> void {
        if (!n) return;
        if (d > stats.depth) stats.depth = d;
        if (n->is_object()) {
            stats.obj++;
            if (auto obj = n->as_object()) {
                for (auto it = obj->begin(); it != obj->end(); ++it) self(self, it->value, d + 1);
            }
        } else if (n->is_array()) {
            stats.arr++;
            if (auto arr = n->as_array()) {
                for (auto* child : *arr) self(self, child, d + 1);
            }
        } else if (n->is_string()) stats.str++;
        else if (n->is_number()) stats.num++;
        else if (n->is_bool()) stats.bool_val++;
        else if (n->is_null()) stats.null_val++;
    };
    
    traverse(traverse, node, 0);
    
    std::cout << "JSON Statistics:\n"
              << "  Max Depth: " << stats.depth << "\n"
              << "  Objects:   " << stats.obj << "\n"
              << "  Arrays:    " << stats.arr << "\n"
              << "  Strings:   " << stats.str << "\n"
              << "  Numbers:   " << stats.num << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    std::string command = argv[1];
    std::string file_path = argv[2];

    try {
        json::Arena arena;
        std::string content = read_input(file_path);
        
        auto result = json::parse(std::span(content.data(), content.size()), arena);
        if (!result) {
            std::cerr << std::format("Error: {} (offset: {})\n", 
                result.error().message, result.error().offset);
            return 1;
        }

        if (command == "validate") {
            std::cout << "Valid JSON.\n";
        } else if (command == "format") {
            std::cout << json::write(*result, true) << "\n";
        } else if (command == "minify") {
            std::cout << json::write(*result, false) << "\n";
        } else if (command == "stats") {
            print_stats(*result);
        } else {
            std::cerr << "Unknown command: " << command << "\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal: " << e.what() << "\n";
        return 1;
    }
    return 0;
}