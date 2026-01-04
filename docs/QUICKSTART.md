# Quickstart Guide

## 1. Parsing JSON

The core of `cpp_json` is the `Arena` allocator. You must create one before parsing.

```cpp
#include "json/api.hpp"

int main() {
    json::Arena arena;
    std::string_view data = R"({"hero": "Zuu", "level": 99})";

    auto result = json::parse(data, arena);
    
    if (!result) {
        // Handle error safely
        return 1;
    }

    json::Node* root = *result;
    // ... use root
}

```

## 2. Accessing Data

Use the type-safe accessors (`as_string`, `as_number`, etc.).

```cpp
// Safe access with std::expected
if (auto hero = (*root)["hero"]) {
    if (auto name = (*hero)->as_string()) {
        std::cout << "Hero: " << *name << "\n"; // Output: Hero: Zuu
    }
}

```

## 3. Creating JSON

Use the Builder API for safe construction.

```cpp
#include "json/builder.hpp"

auto builder = json::build_object(arena);
builder.add("status", "active")
       .add("score", 100);

json::Node* json_out = builder.build();
std::string output = json::write(json_out, true); // true for pretty-print

```