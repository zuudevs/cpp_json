#include "json/api.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>

struct Options {
    std::string input_file;
    bool pretty = false;
    bool validate_only = false;
    bool show_stats = false;
    bool minify = false;
};

void print_usage(const char* program_name) {
    std::cout << "JSON Reader & Validator\n\n";
    std::cout << "Usage: " << program_name << " [OPTIONS] <input-file>\n\n";
    std::cout << "Options:\n";
    std::cout << "  -p, --pretty         Pretty-print JSON output\n";
    std::cout << "  -m, --minify         Minify JSON (remove whitespace)\n";
    std::cout << "  -v, --validate       Validate JSON only (no output)\n";
    std::cout << "  -s, --stats          Show statistics about the JSON\n";
    std::cout << "  -h, --help           Show this help message\n";
    std::cout << "  -                    Read from stdin\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program_name << " -p input.json\n";
    std::cout << "  " << program_name << " --validate data.json\n";
    std::cout << "  cat file.json | " << program_name << " -p -\n";
}

bool parse_args(int argc, char* argv[], Options& opts) {
    if (argc < 2) {
        return false;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            return false;
        } else if (arg == "-p" || arg == "--pretty") {
            opts.pretty = true;
        } else if (arg == "-m" || arg == "--minify") {
            opts.minify = true;
        } else if (arg == "-v" || arg == "--validate") {
            opts.validate_only = true;
        } else if (arg == "-s" || arg == "--stats") {
            opts.show_stats = true;
        } else if (arg[0] != '-') {
            opts.input_file = arg;
        } else if (arg == "-") {
            opts.input_file = "-";
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            return false;
        }
    }

    if (opts.input_file.empty()) {
        std::cerr << "Error: No input file specified\n";
        return false;
    }

    if (opts.pretty && opts.minify) {
        std::cerr << "Error: Cannot use both --pretty and --minify\n";
        return false;
    }

    return true;
}

std::string read_file(const std::string& filename) {
    if (filename == "-") {
        // Read from stdin
        std::ostringstream ss;
        ss << std::cin.rdbuf();
        return ss.str();
    }

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void count_nodes(const json::Node* node, size_t& total, size_t& objects, 
                 size_t& arrays, size_t& strings, size_t& numbers, 
                 size_t& bools, size_t& nulls, size_t& max_depth, size_t depth = 0) {
    if (!node) return;
    
    ++total;
    max_depth = std::max(max_depth, depth);

    switch (node->type) {
        case json::NodeType::Null:
            ++nulls;
            break;
        case json::NodeType::Bool:
            ++bools;
            break;
        case json::NodeType::Number:
            ++numbers;
            break;
        case json::NodeType::String:
            ++strings;
            break;
        case json::NodeType::Array:
            ++arrays;
            for (size_t i = 0; i < node->array_val.size; ++i) {
                count_nodes(node->array_val.data[i], total, objects, arrays, 
                           strings, numbers, bools, nulls, max_depth, depth + 1);
            }
            break;
        case json::NodeType::Object:
            ++objects;
            for (size_t i = 0; i < node->object_val.size; ++i) {
                count_nodes(node->object_val.data[i].value, total, objects, arrays,
                           strings, numbers, bools, nulls, max_depth, depth + 1);
            }
            break;
    }
}

void print_stats(const json::Node* root) {
    size_t total = 0, objects = 0, arrays = 0, strings = 0;
    size_t numbers = 0, bools = 0, nulls = 0, max_depth = 0;
    
    count_nodes(root, total, objects, arrays, strings, numbers, bools, nulls, max_depth);
    
    std::cout << "\nJSON Statistics:\n";
    std::cout << "  Total nodes:    " << total << "\n";
    std::cout << "  Objects:        " << objects << "\n";
    std::cout << "  Arrays:         " << arrays << "\n";
    std::cout << "  Strings:        " << strings << "\n";
    std::cout << "  Numbers:        " << numbers << "\n";
    std::cout << "  Booleans:       " << bools << "\n";
    std::cout << "  Nulls:          " << nulls << "\n";
    std::cout << "  Max depth:      " << max_depth << "\n";
}

int main(int argc, char* argv[]) {
    Options opts;
    
    if (!parse_args(argc, argv, opts)) {
        print_usage(argv[0]);
        return 1;
    }

    try {
        // Read input
        std::string content = read_file(opts.input_file);
        
        // Parse JSON
        json::Arena arena;
        auto result = json::parse(std::span{content.data(), content.size()}, arena);
        
        if (!result) {
            std::cerr << "JSON Parse Error:\n";
            std::cerr << "  Message: " << result.error().message << "\n";
            std::cerr << "  Offset:  " << result.error().offset << "\n";
            
            // Show context around error
            if (result.error().offset < content.size()) {
                size_t start = (result.error().offset > 40) ? result.error().offset - 40 : 0;
                size_t end = std::min(result.error().offset + 40, content.size());
                std::cerr << "  Context: \"" << content.substr(start, end - start) << "\"\n";
                std::cerr << "           " << std::string(result.error().offset - start + 1, ' ') << "^\n";
            }
            return 1;
        }

        json::Node* root = *result;

        if (opts.validate_only) {
            std::cout << "✓ JSON is valid\n";
        } else {
            // Output JSON
            bool use_pretty = opts.pretty || (!opts.minify && !opts.pretty);
            std::string output = json::write(root, use_pretty);
            std::cout << output << "\n";
        }

        if (opts.show_stats) {
            print_stats(root);
        }

        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}