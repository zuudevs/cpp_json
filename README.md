# cpp_json v2.0 - Modern C++23 JSON Parser

A high-performance, zero-copy JSON parser with modern C++23 features and comprehensive safety guarantees.

## Features

✅ **Zero-copy parsing** with arena allocation  
✅ **C++23 iterators** and ranges support  
✅ **Type-safe accessors** using `std::expected`  
✅ **Fluent builder API** for safe construction  
✅ **Proper string unescaping** (including Unicode)  
✅ **No exceptions, no RTTI** - suitable for embedded systems  
✅ **File I/O utilities** with arena integration  

## Quick Start

### Basic Parsing with Safe Accessors

```cpp
#include "json/api.hpp"

using namespace json;

Arena arena;
auto result = parse(R"({"name": "Alice", "age": 30})", arena);

if (result) {
    Node* root = *result;
    
    // Type-safe access with error handling
    if (auto name = (*root)["name"]) {
        if (auto str = (*name)->as_string()) {
            std::cout << "Name: " << *str << "\n";
        }
    }
}
```

### Modern Iterators & Range-Based Loops

```cpp
// Arrays
if (auto arr = node->as_array()) {
    for (Node* item : *arr) {
        // Process each item
    }
}

// Objects with structured bindings
if (auto obj = node->as_object()) {
    for (const auto& [key, value] : *obj) {
        std::cout << key.view() << ": " << value << "\n";
    }
}
```

### Safe Builder API

```cpp
#include "json/builder.hpp"

Arena arena;

// Build objects fluently
auto obj = build_object(arena);
obj.add("user", "Bob")
   .add("id", 12345)
   .add("active", true);

// Build nested structures
auto tags = build_array(arena);
tags.add("developer").add("admin");
obj.add("tags", tags.build());

Node* root = obj.build();
```

### File I/O Integration

```cpp
Arena arena;

// Read and parse file in one call
auto result = parse_file("config.json", arena);

if (result) {
    Node* config = *result;
    // Use config...
}
```

## API Reference

### Type Predicates

```cpp
bool is_null() const;
bool is_bool() const;
bool is_number() const;
bool is_string() const;
bool is_array() const;
bool is_object() const;
```

### Safe Accessors

All accessors return `std::expected<T, AccessError>`:

```cpp
std::expected<bool, AccessError> as_bool() const;
std::expected<double, AccessError> as_number() const;
std::expected<std::string_view, AccessError> as_string() const;
std::expected<ArrayView, AccessError> as_array() const;
std::expected<ObjectView, AccessError> as_object() const;
```

### Operators

```cpp
// Array access (bounds-checked)
std::expected<Node*, AccessError> operator[](size_t index) const;

// Object access (key lookup)
std::expected<Node*, AccessError> operator[](std::string_view key) const;
```

### Builder API

```cpp
// Objects
ObjectBuilder build_object(Arena& arena);
ObjectBuilder& add(std::string_view key, T value);  // T = bool, double, string, Node*
Node* build();

// Arrays
ArrayBuilder build_array(Arena& arena);
ArrayBuilder& add(T value);  // T = bool, double, string, Node*
Node* build();

// Convenience factories
Node* make_null(Arena& arena);
Node* make_bool(Arena& arena, bool value);
Node* make_number(Arena& arena, double value);
Node* make_string(Arena& arena, std::string_view value);
```

### File Operations

```cpp
Result<Node*> parse_file(const char* filename, Arena& arena);
Result<Node*> read_file_to_arena(std::string_view filename, Arena& arena);
```

## String Unescaping

The parser properly handles all JSON escape sequences:

- `\n`, `\t`, `\r`, `\b`, `\f` - Control characters
- `\"`, `\\`, `\/` - Literal characters
- `\uXXXX` - Unicode codepoints (including surrogate pairs)

**Performance**: Zero-copy fast path when no escapes are present. Only allocates when escape sequences are detected.

## Error Handling

All errors use `std::expected` for zero-overhead error handling:

```cpp
auto result = node->as_string();
if (!result) {
    switch (result.error().code) {
        case AccessError::Code::TypeMismatch:
            // Handle type error
            break;
        case AccessError::Code::OutOfBounds:
            // Handle bounds error
            break;
        case AccessError::Code::KeyNotFound:
            // Handle missing key
            break;
    }
}
```

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

The executables will be in the `bin/` directory:

- `json-example` - Basic functionality tests
- `json-examples` - Comprehensive feature demonstrations
- `json-reader` - CLI tool for validation and formatting
- `json-writer` - CLI tool for template generation

## Command-Line Tools

### json-reader

```bash
# Validate JSON
./bin/json-reader --validate data.json

# Pretty-print
./bin/json-reader --pretty data.json

# Show statistics
./bin/json-reader --stats data.json

# Read from stdin
cat data.json | ./bin/json-reader --pretty -
```

### json-writer

```bash
# Generate templates
./bin/json-writer --template object -o output.json
./bin/json-writer --template config
./bin/json-writer --template package
```

## Performance Characteristics

- **Parsing**: O(n) single-pass tokenization and parsing
- **Memory**: Single arena allocation, no fragmentation
- **String handling**: Zero-copy for non-escaped strings
- **Access**: O(1) for arrays, O(n) for object key lookup
- **No heap allocations** outside of Arena

## Safety Guarantees

✅ No buffer overflows (builder API checks bounds)  
✅ No dangling pointers (arena lifetime management)  
✅ No type confusion (std::expected accessors)  
✅ No exceptions or RTTI (suitable for embedded systems)  

## License

MIT License - See LICENSE file for details

## Contributing

This is a reference implementation demonstrating modern C++23 techniques. Feel free to adapt for your projects!

## Version History

### v2.0.0 (Current)
- C++23 iterators and ranges
- Type-safe accessor API
- Fluent builder API
- Proper string unescaping
- File I/O utilities

### v1.0.0
- Initial zero-copy parser implementation