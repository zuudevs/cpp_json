#include "json/api.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

struct WriterOptions {
    std::string output_file;
    bool pretty = true;
    bool from_stdin = false;
    std::string template_type;
};

void print_writer_usage(const char* program_name) {
    std::cout << "JSON Writer & Generator\n\n";
    std::cout << "Usage: " << program_name << " [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  -o, --output FILE    Output file (default: stdout)\n";
    std::cout << "  -m, --minify         Output minified JSON\n";
    std::cout << "  -t, --template TYPE  Generate template JSON\n";
    std::cout << "                       Types: object, array, config, package\n";
    std::cout << "  -h, --help           Show this help message\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program_name << " --template object -o output.json\n";
    std::cout << "  " << program_name << " --template config\n";
}

bool parse_writer_args(int argc, char* argv[], WriterOptions& opts) {
    if (argc < 2) {
        opts.template_type = "object";
        return true;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            return false;
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                opts.output_file = argv[++i];
            } else {
                std::cerr << "Error: " << arg << " requires an argument\n";
                return false;
            }
        } else if (arg == "-m" || arg == "--minify") {
            opts.pretty = false;
        } else if (arg == "-t" || arg == "--template") {
            if (i + 1 < argc) {
                opts.template_type = argv[++i];
            } else {
                std::cerr << "Error: " << arg << " requires an argument\n";
                return false;
            }
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            return false;
        }
    }

    return true;
}

json::Node* create_template_object(json::Arena& arena) {
    // Create: { "key": "value", "number": 42, "active": true }
    auto* pairs = arena.alloc<json::ObjectPair>(3);
    
    pairs[0] = json::ObjectPair{{"key", 3}, arena.alloc<json::Node>()};
    *pairs[0].value = json::Node::make_string("value", 5);
    
    pairs[1] = json::ObjectPair{{"number", 6}, arena.alloc<json::Node>()};
    *pairs[1].value = json::Node::make_number(42.0);
    
    pairs[2] = json::ObjectPair{{"active", 6}, arena.alloc<json::Node>()};
    *pairs[2].value = json::Node::make_bool(true);
    
    auto* node = arena.alloc<json::Node>();
    *node = json::Node::make_object(pairs, 3);
    return node;
}

json::Node* create_template_array(json::Arena& arena) {
    // Create: [1, 2, 3, "four", null]
    auto* items = arena.alloc<json::Node*>(5);
    
    items[0] = arena.alloc<json::Node>();
    *items[0] = json::Node::make_number(1.0);
    
    items[1] = arena.alloc<json::Node>();
    *items[1] = json::Node::make_number(2.0);
    
    items[2] = arena.alloc<json::Node>();
    *items[2] = json::Node::make_number(3.0);
    
    items[3] = arena.alloc<json::Node>();
    *items[3] = json::Node::make_string("four", 4);
    
    items[4] = arena.alloc<json::Node>();
    *items[4] = json::Node::make_null();
    
    auto* node = arena.alloc<json::Node>();
    *node = json::Node::make_array(items, 5);
    return node;
}

json::Node* create_template_config(json::Arena& arena) {
    // Create a config-style JSON
    auto* pairs = arena.alloc<json::ObjectPair>(4);
    
    // "name": "my-app"
    pairs[0] = json::ObjectPair{{"name", 4}, arena.alloc<json::Node>()};
    *pairs[0].value = json::Node::make_string("my-app", 6);
    
    // "version": "1.0.0"
    pairs[1] = json::ObjectPair{{"version", 7}, arena.alloc<json::Node>()};
    *pairs[1].value = json::Node::make_string("1.0.0", 5);
    
    // "port": 8080
    pairs[2] = json::ObjectPair{{"port", 4}, arena.alloc<json::Node>()};
    *pairs[2].value = json::Node::make_number(8080.0);
    
    // "features": ["auth", "api", "cache"]
    auto* features = arena.alloc<json::Node*>(3);
    features[0] = arena.alloc<json::Node>();
    *features[0] = json::Node::make_string("auth", 4);
    features[1] = arena.alloc<json::Node>();
    *features[1] = json::Node::make_string("api", 3);
    features[2] = arena.alloc<json::Node>();
    *features[2] = json::Node::make_string("cache", 5);
    
    pairs[3] = json::ObjectPair{{"features", 8}, arena.alloc<json::Node>()};
    *pairs[3].value = json::Node::make_array(features, 3);
    
    auto* node = arena.alloc<json::Node>();
    *node = json::Node::make_object(pairs, 4);
    return node;
}

json::Node* create_template_package(json::Arena& arena) {
    // Create a package.json-style template
    auto* pairs = arena.alloc<json::ObjectPair>(5);
    
    pairs[0] = json::ObjectPair{{"name", 4}, arena.alloc<json::Node>()};
    *pairs[0].value = json::Node::make_string("my-package", 10);
    
    pairs[1] = json::ObjectPair{{"version", 7}, arena.alloc<json::Node>()};
    *pairs[1].value = json::Node::make_string("1.0.0", 5);
    
    pairs[2] = json::ObjectPair{{"description", 11}, arena.alloc<json::Node>()};
    *pairs[2].value = json::Node::make_string("A sample package", 16);
    
    pairs[3] = json::ObjectPair{{"main", 4}, arena.alloc<json::Node>()};
    *pairs[3].value = json::Node::make_string("index.js", 8);
    
    // "scripts": { "test": "jest", "build": "webpack" }
    auto* scripts_pairs = arena.alloc<json::ObjectPair>(2);
    scripts_pairs[0] = json::ObjectPair{{"test", 4}, arena.alloc<json::Node>()};
    *scripts_pairs[0].value = json::Node::make_string("jest", 4);
    scripts_pairs[1] = json::ObjectPair{{"build", 5}, arena.alloc<json::Node>()};
    *scripts_pairs[1].value = json::Node::make_string("webpack", 7);
    
    pairs[4] = json::ObjectPair{{"scripts", 7}, arena.alloc<json::Node>()};
    *pairs[4].value = json::Node::make_object(scripts_pairs, 2);
    
    auto* node = arena.alloc<json::Node>();
    *node = json::Node::make_object(pairs, 5);
    return node;
}

int main(int argc, char* argv[]) {
    WriterOptions opts;
    
    if (!parse_writer_args(argc, argv, opts)) {
        print_writer_usage(argv[0]);
        return 1;
    }

    try {
        json::Arena arena;
        json::Node* root = nullptr;

        // Create template based on type
        if (opts.template_type == "object") {
            root = create_template_object(arena);
        } else if (opts.template_type == "array") {
            root = create_template_array(arena);
        } else if (opts.template_type == "config") {
            root = create_template_config(arena);
        } else if (opts.template_type == "package") {
            root = create_template_package(arena);
        } else {
            std::cerr << "Unknown template type: " << opts.template_type << "\n";
            std::cerr << "Available types: object, array, config, package\n";
            return 1;
        }

        // Write JSON
        std::string output = json::write(root, opts.pretty);

        if (opts.output_file.empty()) {
            std::cout << output << "\n";
        } else {
            std::ofstream file(opts.output_file);
            if (!file) {
                throw std::runtime_error("Cannot open output file: " + opts.output_file);
            }
            file << output << "\n";
            std::cout << "Written to: " << opts.output_file << "\n";
        }

        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}