#pragma once

#include <cstdint>
#include <cstddef>
#include <string_view>

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

// Trivial string view replacement (aggregate type, no constructors)
struct StringView {
    const char* data;
    size_t size;

    constexpr std::string_view view() const { return {data, size}; }
    constexpr operator std::string_view() const { return view(); }
};

// Trivial array view (aggregate type, no constructors)
struct ArrayView {
    Node** data;
    size_t size;
};

struct ObjectPair {
    StringView key;
    Node* value;
};

// Trivial object view (aggregate type, no constructors)
struct ObjectView {
    ObjectPair* data;
    size_t size;
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
};

static_assert(std::is_trivial_v<StringView>, "StringView must be trivial");
static_assert(std::is_trivial_v<ArrayView>, "ArrayView must be trivial");
static_assert(std::is_trivial_v<ObjectView>, "ObjectView must be trivial");
static_assert(std::is_trivial_v<ObjectPair>, "ObjectPair must be trivial");
static_assert(std::is_trivial_v<Node>, "Node must be trivial");

} // namespace json