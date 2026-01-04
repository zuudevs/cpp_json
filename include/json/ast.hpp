/**
 * @file ast.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief JSON Abstract Syntax Tree (AST) definitions.
 * 
 * This file defines the core Node structure and helper types used to represent
 * the parsed JSON data in memory.
 * @version 1.1.1
 * @date 2026-01-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <string_view>
#include <expected>
#include <iterator>

namespace json {

/**
 * @brief Enumeration of supported JSON value types.
 */
enum class NodeType : uint8_t {
    Null,   ///< JSON null value
    Bool,   ///< JSON boolean (true/false)
    Number, ///< JSON number (double precision)
    String, ///< JSON string
    Array,  ///< JSON array (ordered list)
    Object  ///< JSON object (key-value pairs)
};

struct Node;

/**
 * @brief Error information for failed node access operations.
 */
struct AccessError {
    /**
     * @brief Specific error codes for access failures.
     */
    enum class Code { 
        TypeMismatch,   ///< Requested type does not match actual node type
        OutOfBounds,    ///< Array index out of valid range
        KeyNotFound     ///< Object key does not exist
    };
    
    Code code;                  ///< The error code
    std::string_view message;   ///< Descriptive error message
};

/**
 * @brief A trivial string view wrapper.
 * 
 * Used to ensure the Node struct remains a trivial type (aggregate)
 * while providing conversion to std::string_view.
 */
struct StringView {
    const char* data;   ///< Pointer to the character data
    size_t size;        ///< Length of the string

    /// Converts to std::string_view
    constexpr std::string_view view() const { return {data, size}; }
    /// Implicit conversion operator to std::string_view
    constexpr operator std::string_view() const { return view(); }
};

// Forward declarations for circular dependencies
struct ArrayView;
struct ObjectView;

/**
 * @brief Represents a key-value pair in a JSON object.
 */
struct ObjectPair {
    StringView key; ///< The object key
    Node* value;    ///< The associated value
};

/**
 * @brief Iterator for traversing JSON arrays.
 * 
 * Compliant with C++ LegacyRandomAccessIterator requirements.
 */
struct ArrayIterator {
    using iterator_category = std::random_access_iterator_tag;
    using value_type = Node*;
    using difference_type = std::ptrdiff_t;
    using pointer = Node**;
    using reference = Node*&;

    Node** ptr;

    constexpr ArrayIterator(Node** p) : ptr(p) {}
    
    constexpr Node*& operator*() const { return *ptr; }
    constexpr Node** operator->() const { return ptr; }
    constexpr ArrayIterator& operator++() { ++ptr; return *this; }
    constexpr ArrayIterator operator++(int) { auto tmp = *this; ++ptr; return tmp; }
    constexpr ArrayIterator& operator--() { --ptr; return *this; }
    constexpr ArrayIterator operator--(int) { auto tmp = *this; --ptr; return tmp; }
    constexpr ArrayIterator& operator+=(difference_type n) { ptr += n; return *this; }
    constexpr ArrayIterator& operator-=(difference_type n) { ptr -= n; return *this; }
    constexpr ArrayIterator operator+(difference_type n) const { return ArrayIterator(ptr + n); }
    constexpr ArrayIterator operator-(difference_type n) const { return ArrayIterator(ptr - n); }
    constexpr difference_type operator-(const ArrayIterator& other) const { return ptr - other.ptr; }
    constexpr Node*& operator[](difference_type n) const { return ptr[n]; }
    
    constexpr bool operator==(const ArrayIterator& other) const { return ptr == other.ptr; }
    constexpr auto operator<=>(const ArrayIterator& other) const { return ptr <=> other.ptr; }
};

/**
 * @brief A lightweight view over a JSON array.
 * 
 * Provides iteration and random access to array elements.
 */
struct ArrayView {
    Node** data;    ///< Pointer to the array of Node pointers
    size_t size;    ///< Number of elements in the array

    /// Returns an iterator to the beginning of the array
    constexpr ArrayIterator begin() const { return ArrayIterator(data); }
    /// Returns an iterator to the end of the array
    constexpr ArrayIterator end() const { return ArrayIterator(data + size); }
    
    /// Safe random access (returns nullptr if out of bounds)
    constexpr Node* operator[](size_t index) const {
        return index < size ? data[index] : nullptr;
    }
    
    /// Checks if the array is empty
    constexpr bool empty() const { return size == 0; }
};

/**
 * @brief Iterator for traversing JSON objects.
 * 
 * Compliant with C++ LegacyRandomAccessIterator requirements.
 */
struct ObjectIterator {
    using iterator_category = std::random_access_iterator_tag;
    using value_type = ObjectPair;
    using difference_type = std::ptrdiff_t;
    using pointer = ObjectPair*;
    using reference = ObjectPair&;

    ObjectPair* ptr;

    constexpr ObjectIterator(ObjectPair* p) : ptr(p) {}
    
    constexpr ObjectPair& operator*() const { return *ptr; }
    constexpr ObjectPair* operator->() const { return ptr; }
    constexpr ObjectIterator& operator++() { ++ptr; return *this; }
    constexpr ObjectIterator operator++(int) { auto tmp = *this; ++ptr; return tmp; }
    constexpr ObjectIterator& operator--() { --ptr; return *this; }
    constexpr ObjectIterator operator--(int) { auto tmp = *this; --ptr; return tmp; }
    constexpr ObjectIterator& operator+=(difference_type n) { ptr += n; return *this; }
    constexpr ObjectIterator& operator-=(difference_type n) { ptr -= n; return *this; }
    constexpr ObjectIterator operator+(difference_type n) const { return ObjectIterator(ptr + n); }
    constexpr ObjectIterator operator-(difference_type n) const { return ObjectIterator(ptr - n); }
    constexpr difference_type operator-(const ObjectIterator& other) const { return ptr - other.ptr; }
    constexpr ObjectPair& operator[](difference_type n) const { return ptr[n]; }
    
    constexpr bool operator==(const ObjectIterator& other) const { return ptr == other.ptr; }
    constexpr auto operator<=>(const ObjectIterator& other) const { return ptr <=> other.ptr; }
};

/**
 * @brief A lightweight view over a JSON object.
 * 
 * Provides iteration and key-based lookup.
 */
struct ObjectView {
    ObjectPair* data;   ///< Pointer to the array of key-value pairs
    size_t size;        ///< Number of pairs in the object

    /// Returns an iterator to the beginning of the object pairs
    constexpr ObjectIterator begin() const { return ObjectIterator(data); }
    /// Returns an iterator to the end of the object pairs
    constexpr ObjectIterator end() const { return ObjectIterator(data + size); }
    
    /// Checks if the object is empty
    constexpr bool empty() const { return size == 0; }
    
    /// Finds a value by key (linear search). Returns nullptr if not found.
    constexpr Node* find(std::string_view key) const {
        for (size_t i = 0; i < size; ++i) {
            if (data[i].key.view() == key) {
                return data[i].value;
            }
        }
        return nullptr;
    }
};

/**
 * @brief The fundamental node in the JSON Abstract Syntax Tree (AST).
 * 
 * A Node represents a single JSON value. It is a tagged union that can hold
 * any of the supported JSON types. It is designed to be a trivial type
 * (POD) to allow for efficient allocation in an Arena.
 */
struct Node {
    NodeType type; ///< The type of value stored in this node
    
    /**
     * @brief Union of all possible value types.
     * Only the member corresponding to 'type' is valid.
     */
    union {
        bool bool_val;          ///< Valid if type == NodeType::Bool
        double number_val;      ///< Valid if type == NodeType::Number
        StringView string_val;  ///< Valid if type == NodeType::String
        ArrayView array_val;    ///< Valid if type == NodeType::Array
        ObjectView object_val;  ///< Valid if type == NodeType::Object
    };

    /**
     * @brief Creates a null node.
     * @return Node A new node of type Null.
     */
    static Node make_null() {
        return Node{.type = NodeType::Null, .bool_val = false};
    }

    /**
     * @brief Creates a boolean node.
     * @param val The boolean value.
     * @return Node A new node of type Bool.
     */
    static Node make_bool(bool val) {
        return Node{.type = NodeType::Bool, .bool_val = val};
    }

    /**
     * @brief Creates a number node.
     * @param val The numeric value.
     * @return Node A new node of type Number.
     */
    static Node make_number(double val) {
        return Node{.type = NodeType::Number, .number_val = val};
    }

    /**
     * @brief Creates a string node.
     * @param data Pointer to the string data.
     * @param size Length of the string.
     * @return Node A new node of type String.
     */
    static Node make_string(const char* data, size_t size) {
        return Node{.type = NodeType::String, .string_val = {data, size}};
    }

    /**
     * @brief Creates an array node.
     * @param data Pointer to the array of Node pointers.
     * @param size Number of elements.
     * @return Node A new node of type Array.
     */
    static Node make_array(Node** data, size_t size) {
        return Node{.type = NodeType::Array, .array_val = {data, size}};
    }

    /**
     * @brief Creates an object node.
     * @param data Pointer to the array of ObjectPairs.
     * @param size Number of pairs.
     * @return Node A new node of type Object.
     */
    static Node make_object(ObjectPair* data, size_t size) {
        return Node{.type = NodeType::Object, .object_val = {data, size}};
    }

    /// @brief Checks if the node is Null.
    constexpr bool is_null() const { return type == NodeType::Null; }
    /// @brief Checks if the node is a Boolean.
    constexpr bool is_bool() const { return type == NodeType::Bool; }
    /// @brief Checks if the node is a Number.
    constexpr bool is_number() const { return type == NodeType::Number; }
    /// @brief Checks if the node is a String.
    constexpr bool is_string() const { return type == NodeType::String; }
    /// @brief Checks if the node is an Array.
    constexpr bool is_array() const { return type == NodeType::Array; }
    /// @brief Checks if the node is an Object.
    constexpr bool is_object() const { return type == NodeType::Object; }

    /**
     * @brief Attempts to access the node as a boolean.
     * 
     * @return std::expected<bool, AccessError> The boolean value or an error.
     */
    constexpr std::expected<bool, AccessError> as_bool() const {
        if (type != NodeType::Bool) {
            return std::unexpected(AccessError{
                AccessError::Code::TypeMismatch,
                "Node is not a boolean"
            });
        }
        return bool_val;
    }

    /**
     * @brief Attempts to access the node as a number.
     * 
     * @return std::expected<double, AccessError> The numeric value or an error.
     */
    constexpr std::expected<double, AccessError> as_number() const {
        if (type != NodeType::Number) {
            return std::unexpected(AccessError{
                AccessError::Code::TypeMismatch,
                "Node is not a number"
            });
        }
        return number_val;
    }

    /**
     * @brief Attempts to access the node as a string.
     * 
     * @return std::expected<std::string_view, AccessError> The string view or an error.
     */
    constexpr std::expected<std::string_view, AccessError> as_string() const {
        if (type != NodeType::String) {
            return std::unexpected(AccessError{
                AccessError::Code::TypeMismatch,
                "Node is not a string"
            });
        }
        return string_val.view();
    }

    /**
     * @brief Attempts to access the node as an array view.
     * 
     * @return std::expected<ArrayView, AccessError> The array view or an error.
     */
    constexpr std::expected<ArrayView, AccessError> as_array() const {
        if (type != NodeType::Array) {
            return std::unexpected(AccessError{
                AccessError::Code::TypeMismatch,
                "Node is not an array"
            });
        }
        return array_val;
    }

    /**
     * @brief Attempts to access the node as an object view.
     * 
     * @return std::expected<ObjectView, AccessError> The object view or an error.
     */
    constexpr std::expected<ObjectView, AccessError> as_object() const {
        if (type != NodeType::Object) {
            return std::unexpected(AccessError{
                AccessError::Code::TypeMismatch,
                "Node is not an object"
            });
        }
        return object_val;
    }

    /**
     * @brief Accesses an array element by index with bounds checking.
     * 
     * @param index The index to access.
     * @return std::expected<Node*, AccessError> Pointer to the node or an error.
     */
    constexpr std::expected<Node*, AccessError> operator[](size_t index) const {
        if (type != NodeType::Array) {
            return std::unexpected(AccessError{
                AccessError::Code::TypeMismatch,
                "Node is not an array"
            });
        }
        if (index >= array_val.size) {
            return std::unexpected(AccessError{
                AccessError::Code::OutOfBounds,
                "Array index out of bounds"
            });
        }
        return array_val.data[index];
    }

    /**
     * @brief Accesses an object value by key.
     * 
     * @param key The key to look up.
     * @return std::expected<Node*, AccessError> Pointer to the value node or an error.
     */
    constexpr std::expected<Node*, AccessError> operator[](std::string_view key) const {
        if (type != NodeType::Object) {
            return std::unexpected(AccessError{
                AccessError::Code::TypeMismatch,
                "Node is not an object"
            });
        }
        Node* result = object_val.find(key);
        if (!result) {
            return std::unexpected(AccessError{
                AccessError::Code::KeyNotFound,
                "Key not found in object"
            });
        }
        return result;
    }

    /**
     * @brief Gets the size of the array or object.
     * 
     * @return size_t The number of elements/pairs, or 0 for other types.
     */
    constexpr size_t size() const {
        if (type == NodeType::Array) return array_val.size;
        if (type == NodeType::Object) return object_val.size;
        return 0;
    }
};

static_assert(std::is_trivial_v<StringView>, "StringView must be trivial");
static_assert(std::is_trivial_v<ArrayView>, "ArrayView must be trivial");
static_assert(std::is_trivial_v<ObjectView>, "ObjectView must be trivial");
static_assert(std::is_trivial_v<ObjectPair>, "ObjectPair must be trivial");
static_assert(std::is_trivial_v<Node>, "Node must be trivial");

} // namespace json
