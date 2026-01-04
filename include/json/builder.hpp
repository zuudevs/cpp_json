#pragma once

#include "ast.hpp"
#include "arena.hpp"
#include <vector>
#include <string_view>
#include <initializer_list>
#include <cstring>

namespace json {

// Forward declarations
class ObjectBuilder;
class ArrayBuilder;

// Helper to allocate and copy a string into the arena
inline const char* arena_copy_string(Arena& arena, std::string_view str) {
    char* buffer = arena.alloc<char>(str.size());
    std::memcpy(buffer, str.data(), str.size());
    return buffer;
}

/**
 * @brief Safe builder for JSON arrays
 * 
 * Example:
 *   ArrayBuilder arr(arena);
 *   arr.add(1.0).add(2.0).add("three");
 *   Node* node = arr.build();
 */
class ArrayBuilder {
public:
    explicit ArrayBuilder(Arena& arena) : arena_(arena) {}

    // Add values with type safety
    ArrayBuilder& add(std::nullptr_t) {
        Node* node = arena_.alloc<Node>();
        *node = Node::make_null();
        elements_.push_back(node);
        return *this;
    }

    ArrayBuilder& add(bool value) {
        Node* node = arena_.alloc<Node>();
        *node = Node::make_bool(value);
        elements_.push_back(node);
        return *this;
    }

    ArrayBuilder& add(double value) {
        Node* node = arena_.alloc<Node>();
        *node = Node::make_number(value);
        elements_.push_back(node);
        return *this;
    }

    ArrayBuilder& add(int value) {
        return add(static_cast<double>(value));
    }

    ArrayBuilder& add(std::string_view value) {
        const char* data = arena_copy_string(arena_, value);
        Node* node = arena_.alloc<Node>();
        *node = Node::make_string(data, value.size());
        elements_.push_back(node);
        return *this;
    }

    ArrayBuilder& add(const char* value) {
        return add(std::string_view(value));
    }

    ArrayBuilder& add(Node* node) {
        if (node) {
            elements_.push_back(node);
        }
        return *this;
    }

    // Build the final array node
    Node* build() {
        Node** arr = arena_.alloc<Node*>(elements_.size());
        for (size_t i = 0; i < elements_.size(); ++i) {
            arr[i] = elements_[i];
        }
        
        Node* node = arena_.alloc<Node>();
        *node = Node::make_array(arr, elements_.size());
        return node;
    }

    size_t size() const { return elements_.size(); }

private:
    Arena& arena_;
    std::vector<Node*> elements_;
};

/**
 * @brief Safe builder for JSON objects
 * 
 * Example:
 *   ObjectBuilder obj(arena);
 *   obj.add("name", "John")
 *      .add("age", 30)
 *      .add("active", true);
 *   Node* node = obj.build();
 */
class ObjectBuilder {
public:
    explicit ObjectBuilder(Arena& arena) : arena_(arena) {}

    // Add key-value pairs
    ObjectBuilder& add(std::string_view key, std::nullptr_t) {
        Node* node = arena_.alloc<Node>();
        *node = Node::make_null();
        add_pair(key, node);
        return *this;
    }

    ObjectBuilder& add(std::string_view key, bool value) {
        Node* node = arena_.alloc<Node>();
        *node = Node::make_bool(value);
        add_pair(key, node);
        return *this;
    }

    ObjectBuilder& add(std::string_view key, double value) {
        Node* node = arena_.alloc<Node>();
        *node = Node::make_number(value);
        add_pair(key, node);
        return *this;
    }

    ObjectBuilder& add(std::string_view key, int value) {
        return add(key, static_cast<double>(value));
    }

    ObjectBuilder& add(std::string_view key, std::string_view value) {
        const char* data = arena_copy_string(arena_, value);
        Node* node = arena_.alloc<Node>();
        *node = Node::make_string(data, value.size());
        add_pair(key, node);
        return *this;
    }

    ObjectBuilder& add(std::string_view key, const char* value) {
        return add(key, std::string_view(value));
    }

    ObjectBuilder& add(std::string_view key, Node* node) {
        if (node) {
            add_pair(key, node);
        }
        return *this;
    }

    // Build the final object node
    Node* build() {
        ObjectPair* obj = arena_.alloc<ObjectPair>(pairs_.size());
        for (size_t i = 0; i < pairs_.size(); ++i) {
            obj[i] = pairs_[i];
        }
        
        Node* node = arena_.alloc<Node>();
        *node = Node::make_object(obj, pairs_.size());
        return node;
    }

    size_t size() const { return pairs_.size(); }

private:
    void add_pair(std::string_view key, Node* value) {
        const char* key_data = arena_copy_string(arena_, key);
        pairs_.push_back(ObjectPair{{key_data, key.size()}, value});
    }

    Arena& arena_;
    std::vector<ObjectPair> pairs_;
};

// Convenience factory functions

/**
 * @brief Create a new ObjectBuilder
 */
inline ObjectBuilder build_object(Arena& arena) {
    return ObjectBuilder(arena);
}

/**
 * @brief Create a new ArrayBuilder
 */
inline ArrayBuilder build_array(Arena& arena) {
    return ArrayBuilder(arena);
}

/**
 * @brief Create an array from initializer list of doubles
 * 
 * Example: auto arr = build_array(arena, {1.0, 2.0, 3.0});
 */
inline Node* build_array(Arena& arena, std::initializer_list<double> values) {
    ArrayBuilder builder(arena);
    for (double val : values) {
        builder.add(val);
    }
    return builder.build();
}

/**
 * @brief Create an array from initializer list of strings
 */
inline Node* build_array(Arena& arena, std::initializer_list<const char*> values) {
    ArrayBuilder builder(arena);
    for (const char* val : values) {
        builder.add(val);
    }
    return builder.build();
}

/**
 * @brief Create primitive nodes directly
 */
inline Node* make_null(Arena& arena) {
    Node* node = arena.alloc<Node>();
    *node = Node::make_null();
    return node;
}

inline Node* make_bool(Arena& arena, bool value) {
    Node* node = arena.alloc<Node>();
    *node = Node::make_bool(value);
    return node;
}

inline Node* make_number(Arena& arena, double value) {
    Node* node = arena.alloc<Node>();
    *node = Node::make_number(value);
    return node;
}

inline Node* make_string(Arena& arena, std::string_view value) {
    const char* data = arena_copy_string(arena, value);
    Node* node = arena.alloc<Node>();
    *node = Node::make_string(data, value.size());
    return node;
}

} // namespace json
