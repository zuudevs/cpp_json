#pragma once

#include <cstdint>
#include <cstddef>
#include <string_view>
#include <expected>
#include <iterator>

namespace json {

enum class NodeType : uint8_t {
    Null,
    Bool,
    Number,
    String,
    Array,
    Object
};

struct Node;

// Error type for accessor failures
struct AccessError {
    enum class Code { TypeMismatch, OutOfBounds, KeyNotFound };
    Code code;
    std::string_view message;
};

// Trivial string view replacement (aggregate type, no constructors)
struct StringView {
    const char* data;
    size_t size;

    constexpr std::string_view view() const { return {data, size}; }
    constexpr operator std::string_view() const { return view(); }
};

// Forward declarations for circular dependencies
struct ArrayView;
struct ObjectView;

// Define ObjectPair first (needed by ObjectIterator)
struct ObjectPair {
    StringView key;
    Node* value;
};

// Array iterator
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

// Trivial array view with iterator support
struct ArrayView {
    Node** data;
    size_t size;

    // Iterator support
    constexpr ArrayIterator begin() const { return ArrayIterator(data); }
    constexpr ArrayIterator end() const { return ArrayIterator(data + size); }
    
    // Random access
    constexpr Node* operator[](size_t index) const {
        return index < size ? data[index] : nullptr;
    }
    
    constexpr bool empty() const { return size == 0; }
};

// Object iterator for structured bindings
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

// Trivial object view with iterator support
struct ObjectView {
    ObjectPair* data;
    size_t size;

    // Iterator support
    constexpr ObjectIterator begin() const { return ObjectIterator(data); }
    constexpr ObjectIterator end() const { return ObjectIterator(data + size); }
    
    constexpr bool empty() const { return size == 0; }
    
    // Key lookup (linear search - acceptable for small objects)
    constexpr Node* find(std::string_view key) const {
        for (size_t i = 0; i < size; ++i) {
            if (data[i].key.view() == key) {
                return data[i].value;
            }
        }
        return nullptr;
    }
};

struct Node {
    NodeType type;
    
    union {
        bool bool_val;
        double number_val;
        StringView string_val;
        ArrayView array_val;
        ObjectView object_val;
    };

    // Factory methods for each type (using aggregate initialization)
    static Node make_null() {
        return Node{.type = NodeType::Null, .bool_val = false};
    }

    static Node make_bool(bool val) {
        return Node{.type = NodeType::Bool, .bool_val = val};
    }

    static Node make_number(double val) {
        return Node{.type = NodeType::Number, .number_val = val};
    }

    static Node make_string(const char* data, size_t size) {
        return Node{.type = NodeType::String, .string_val = {data, size}};
    }

    static Node make_array(Node** data, size_t size) {
        return Node{.type = NodeType::Array, .array_val = {data, size}};
    }

    static Node make_object(ObjectPair* data, size_t size) {
        return Node{.type = NodeType::Object, .object_val = {data, size}};
    }

    // Type predicates
    constexpr bool is_null() const { return type == NodeType::Null; }
    constexpr bool is_bool() const { return type == NodeType::Bool; }
    constexpr bool is_number() const { return type == NodeType::Number; }
    constexpr bool is_string() const { return type == NodeType::String; }
    constexpr bool is_array() const { return type == NodeType::Array; }
    constexpr bool is_object() const { return type == NodeType::Object; }

    // Safe accessors using std::expected
    constexpr std::expected<bool, AccessError> as_bool() const {
        if (type != NodeType::Bool) {
            return std::unexpected(AccessError{
                AccessError::Code::TypeMismatch,
                "Node is not a boolean"
            });
        }
        return bool_val;
    }

    constexpr std::expected<double, AccessError> as_number() const {
        if (type != NodeType::Number) {
            return std::unexpected(AccessError{
                AccessError::Code::TypeMismatch,
                "Node is not a number"
            });
        }
        return number_val;
    }

    constexpr std::expected<std::string_view, AccessError> as_string() const {
        if (type != NodeType::String) {
            return std::unexpected(AccessError{
                AccessError::Code::TypeMismatch,
                "Node is not a string"
            });
        }
        return string_val.view();
    }

    constexpr std::expected<ArrayView, AccessError> as_array() const {
        if (type != NodeType::Array) {
            return std::unexpected(AccessError{
                AccessError::Code::TypeMismatch,
                "Node is not an array"
            });
        }
        return array_val;
    }

    constexpr std::expected<ObjectView, AccessError> as_object() const {
        if (type != NodeType::Object) {
            return std::unexpected(AccessError{
                AccessError::Code::TypeMismatch,
                "Node is not an object"
            });
        }
        return object_val;
    }

    // Array accessor with bounds checking
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

    // Object accessor with key lookup
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

    // Convenience: get array/object size
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
