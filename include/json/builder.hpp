/**
 * @file builder.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief JSON Builder API
 * 
 * This file defines helper classes and functions for constructing JSON AST nodes
 * programmatically using a fluent API.
 * @version 1.0.0
 * @date 2026-01-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */

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

/**
 * @brief Helper to allocate and copy a string into the arena.
 * 
 * @param arena The arena to allocate memory from.
 * @param str The string to copy.
 * @return const char* Pointer to the null-terminated string in the arena.
 */
inline const char* arena_copy_string(Arena& arena, std::string_view str) {
    char* buffer = arena.alloc<char>(str.size());
    std::memcpy(buffer, str.data(), str.size());
    return buffer;
}

/**
 * @brief Safe builder for JSON arrays
 * 
 * Provides a fluent interface for constructing JSON arrays.
 * Allocates nodes from the provided Arena.
 * 
 * Example:
 *   ArrayBuilder arr(arena);
 *   arr.add(1.0).add(2.0).add("three");
 *   Node* node = arr.build();
 */
class ArrayBuilder {
public:
    /**
     * @brief Constructs a new ArrayBuilder.
     * 
     * @param arena The memory arena used for node allocation.
     */
    explicit ArrayBuilder(Arena& arena) : arena_(arena) {}

    /**
     * @brief Appends a null value to the array.
     * 
     * @param value Must be nullptr.
     * @return ArrayBuilder& Reference to self for chaining.
     */
    ArrayBuilder& add(std::nullptr_t) {
        Node* node = arena_.alloc<Node>();
        *node = Node::make_null();
        elements_.push_back(node);
        return *this;
    }

    /**
     * @brief Appends a boolean value to the array.
     * 
     * @param value The boolean value.
     * @return ArrayBuilder& Reference to self for chaining.
     */
    ArrayBuilder& add(bool value) {
        Node* node = arena_.alloc<Node>();
        *node = Node::make_bool(value);
        elements_.push_back(node);
        return *this;
    }

    /**
     * @brief Appends a number to the array.
     * 
     * @param value The numeric value (double).
     * @return ArrayBuilder& Reference to self for chaining.
     */
    ArrayBuilder& add(double value) {
        Node* node = arena_.alloc<Node>();
        *node = Node::make_number(value);
        elements_.push_back(node);
        return *this;
    }

    /**
     * @brief Appends an integer to the array.
     * 
     * @param value The integer value.
     * @return ArrayBuilder& Reference to self for chaining.
     */
    ArrayBuilder& add(int value) {
        return add(static_cast<double>(value));
    }

    /**
     * @brief Appends a string to the array.
     * 
     * @param value The string value.
     * @return ArrayBuilder& Reference to self for chaining.
     */
    ArrayBuilder& add(std::string_view value) {
        const char* data = arena_copy_string(arena_, value);
        Node* node = arena_.alloc<Node>();
        *node = Node::make_string(data, value.size());
        elements_.push_back(node);
        return *this;
    }

    /**
     * @brief Appends a C-string to the array.
     * 
     * @param value The C-string value.
     * @return ArrayBuilder& Reference to self for chaining.
     */
    ArrayBuilder& add(const char* value) {
        return add(std::string_view(value));
    }

    /**
     * @brief Appends an existing Node to the array.
     * 
     * @param node Pointer to the Node to append.
     * @return ArrayBuilder& Reference to self for chaining.
     */
    ArrayBuilder& add(Node* node) {
        if (node) {
            elements_.push_back(node);
        }
        return *this;
    }

    /**
     * @brief Finalizes the array construction.
     * 
     * @return Node* Pointer to the newly created Array Node.
     */
    Node* build() {
        Node** arr = arena_.alloc<Node*>(elements_.size());
        for (size_t i = 0; i < elements_.size(); ++i) {
            arr[i] = elements_[i];
        }
        
        Node* node = arena_.alloc<Node>();
        *node = Node::make_array(arr, elements_.size());
        return node;
    }

    /**
     * @brief Gets the current number of elements in the builder.
     * 
     * @return size_t The number of elements.
     */
    size_t size() const { return elements_.size(); }

private:
    Arena& arena_;
    std::vector<Node*> elements_;
};

/**
 * @brief Safe builder for JSON objects
 * 
 * Provides a fluent interface for constructing JSON objects.
 * Allocates nodes from the provided Arena.
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
    /**
     * @brief Constructs a new ObjectBuilder.
     * 
     * @param arena The memory arena used for node allocation.
     */
    explicit ObjectBuilder(Arena& arena) : arena_(arena) {}

    /**
     * @brief Adds a null value with the specified key.
     * 
     * @param key The object key.
     * @param value Must be nullptr.
     * @return ObjectBuilder& Reference to self for chaining.
     */
    ObjectBuilder& add(std::string_view key, std::nullptr_t) {
        Node* node = arena_.alloc<Node>();
        *node = Node::make_null();
        add_pair(key, node);
        return *this;
    }

    /**
     * @brief Adds a boolean value with the specified key.
     * 
     * @param key The object key.
     * @param value The boolean value.
     * @return ObjectBuilder& Reference to self for chaining.
     */
    ObjectBuilder& add(std::string_view key, bool value) {
        Node* node = arena_.alloc<Node>();
        *node = Node::make_bool(value);
        add_pair(key, node);
        return *this;
    }

    /**
     * @brief Adds a number value with the specified key.
     * 
     * @param key The object key.
     * @param value The numeric value (double).
     * @return ObjectBuilder& Reference to self for chaining.
     */
    ObjectBuilder& add(std::string_view key, double value) {
        Node* node = arena_.alloc<Node>();
        *node = Node::make_number(value);
        add_pair(key, node);
        return *this;
    }

    /**
     * @brief Adds an integer value with the specified key.
     * 
     * @param key The object key.
     * @param value The integer value.
     * @return ObjectBuilder& Reference to self for chaining.
     */
    ObjectBuilder& add(std::string_view key, int value) {
        return add(key, static_cast<double>(value));
    }

    /**
     * @brief Adds a string value with the specified key.
     * 
     * @param key The object key.
     * @param value The string value.
     * @return ObjectBuilder& Reference to self for chaining.
     */
    ObjectBuilder& add(std::string_view key, std::string_view value) {
        const char* data = arena_copy_string(arena_, value);
        Node* node = arena_.alloc<Node>();
        *node = Node::make_string(data, value.size());
        add_pair(key, node);
        return *this;
    }

    /**
     * @brief Adds a C-string value with the specified key.
     * 
     * @param key The object key.
     * @param value The C-string value.
     * @return ObjectBuilder& Reference to self for chaining.
     */
    ObjectBuilder& add(std::string_view key, const char* value) {
        return add(key, std::string_view(value));
    }

    /**
     * @brief Adds an existing Node with the specified key.
     * 
     * @param key The object key.
     * @param node Pointer to the Node to add.
     * @return ObjectBuilder& Reference to self for chaining.
     */
    ObjectBuilder& add(std::string_view key, Node* node) {
        if (node) {
            add_pair(key, node);
        }
        return *this;
    }

    /**
     * @brief Finalizes the object construction.
     * 
     * @return Node* Pointer to the newly created Object Node.
     */
    Node* build() {
        ObjectPair* obj = arena_.alloc<ObjectPair>(pairs_.size());
        for (size_t i = 0; i < pairs_.size(); ++i) {
            obj[i] = pairs_[i];
        }
        
        Node* node = arena_.alloc<Node>();
        *node = Node::make_object(obj, pairs_.size());
        return node;
    }

    /**
     * @brief Gets the current number of key-value pairs in the builder.
     * 
     * @return size_t The number of pairs.
     */
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
 * @brief Creates a new ObjectBuilder.
 * 
 * @param arena The arena to use for allocation.
 * @return ObjectBuilder A new builder instance.
 */
inline ObjectBuilder build_object(Arena& arena) {
    return ObjectBuilder(arena);
}

/**
 * @brief Creates a new ArrayBuilder.
 * 
 * @param arena The arena to use for allocation.
 * @return ArrayBuilder A new builder instance.
 */
inline ArrayBuilder build_array(Arena& arena) {
    return ArrayBuilder(arena);
}

/**
 * @brief Creates an array node from an initializer list of doubles.
 * 
 * Example: auto arr = build_array(arena, {1.0, 2.0, 3.0});
 * 
 * @param arena The arena to use for allocation.
 * @param values The list of double values.
 * @return Node* Pointer to the created Array Node.
 */
inline Node* build_array(Arena& arena, std::initializer_list<double> values) {
    ArrayBuilder builder(arena);
    for (double val : values) {
        builder.add(val);
    }
    return builder.build();
}

/**
 * @brief Creates an array node from an initializer list of C-strings.
 * 
 * @param arena The arena to use for allocation.
 * @param values The list of string values.
 * @return Node* Pointer to the created Array Node.
 */
inline Node* build_array(Arena& arena, std::initializer_list<const char*> values) {
    ArrayBuilder builder(arena);
    for (const char* val : values) {
        builder.add(val);
    }
    return builder.build();
}

/**
 * @brief Creates a null node.
 * 
 * @param arena The arena to use for allocation.
 * @return Node* Pointer to the created Null Node.
 */
inline Node* make_null(Arena& arena) {
    Node* node = arena.alloc<Node>();
    *node = Node::make_null();
    return node;
}

/**
 * @brief Creates a boolean node.
 * 
 * @param arena The arena to use for allocation.
 * @param value The boolean value.
 * @return Node* Pointer to the created Bool Node.
 */
inline Node* make_bool(Arena& arena, bool value) {
    Node* node = arena.alloc<Node>();
    *node = Node::make_bool(value);
    return node;
}

/**
 * @brief Creates a number node.
 * 
 * @param arena The arena to use for allocation.
 * @param value The numeric value.
 * @return Node* Pointer to the created Number Node.
 */
inline Node* make_number(Arena& arena, double value) {
    Node* node = arena.alloc<Node>();
    *node = Node::make_number(value);
    return node;
}

/**
 * @brief Creates a string node.
 * 
 * @param arena The arena to use for allocation.
 * @param value The string value.
 * @return Node* Pointer to the created String Node.
 */
inline Node* make_string(Arena& arena, std::string_view value) {
    const char* data = arena_copy_string(arena, value);
    Node* node = arena.alloc<Node>();
    *node = Node::make_string(data, value.size());
    return node;
}

} // namespace json
