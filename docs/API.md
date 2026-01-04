# API Reference

## `json::Node`

The main AST node.

### Predicates
* `bool is_null() const`
* `bool is_object() const`
* ... (and so on)

### Accessors
Returns `std::expected<T, AccessError>`.
* `as_string()` -> `std::string_view`
* `as_number()` -> `double`
* `as_bool()` -> `bool`
* `as_array()` -> `ArrayView`
* `as_object()` -> `ObjectView`

## `json::Arena`

* `Arena(size_t block_size)`: Constructor.
* `T* alloc<T>(count)`: Allocates `count` items of type `T`.
* `reset()`: Resets the pointer to the beginning (reuse memory).

## `json::parse`

* `Result<Node*> parse(std::string_view input, Arena& arena)`: Parses string.
* `Result<Node*> parse_file(const char* path, Arena& arena)`: Loads and parses file.