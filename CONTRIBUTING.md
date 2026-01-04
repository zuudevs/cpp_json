# Contributing to Cpp Json

Thank you for your interest in contributing to Cpp Json! This document provides guidelines for contributing to the project.

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/your-username/cpp-json.git`
3. Create a feature branch: `git checkout -b feature/your-feature-name`
4. Make your changes
5. Run tests to ensure everything works
6. Commit your changes with clear messages
7. Push to your fork: `git push origin feature/your-feature-name`
8. Open a Pull Request

## Pull Request Guidelines

### Before Submitting

- Ensure your code compiles without warnings on GCC, Clang, and MSVC
- Run all existing tests and add new tests for your changes
- Update documentation if you're changing public APIs
- Follow the C++ style guide (see below)
- Keep commits atomic and write clear commit messages

### Pull Request Process

1. **Title**: Use clear, descriptive titles (e.g., "Add JSON array parsing support")
2. **Description**: Explain what changes you made and why
3. **Tests**: Include test cases that cover your changes
4. **Documentation**: Update relevant documentation files
5. **Breaking Changes**: Clearly mark any breaking changes in the PR description

### PR Review Process

- Maintainers will review your PR within 3-5 business days
- Address review comments promptly
- Once approved, a maintainer will merge your PR

## C++ Style Guide

### Core Principles

**Modern C++**: We use C++23 features. Embrace modern idioms and standard library facilities.

**RAII Everywhere**: Resource Acquisition Is Initialization. All resources must be managed through RAII objects.

**No Naked Pointers**: Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) or references. Raw pointers are only acceptable for non-owning views in performance-critical code with clear documentation.

### Coding Standards

#### Naming Conventions

- **Classes/Structs**: `PascalCase` (e.g., `JsonParser`, `JsonValue`)
- **Functions/Methods**: `snake_case` (e.g., `parse_json()`, `get_value()`)
- **Variables**: `snake_case` (e.g., `json_data`, `node_count`)
- **Constants**: `UPPER_SNAKE_CASE` (e.g., `MAX_DEPTH`, `DEFAULT_BUFFER_SIZE`)
- **Private members**: Prefix with `m_` (e.g., `m_data`, `m_size`)

#### Formatting

```cpp
// Use 4 spaces for indentation (no tabs)
// Braces on same line for functions
void parse_json(std::string_view input) {
    if (input.empty()) {
        return;
    }
    
    // Process input
}

// Class formatting
class JsonParser {
public:
    explicit JsonParser(std::string_view input);
    
    auto parse() -> std::expected<JsonValue, ParseError>;
    
private:
    std::string m_input;
    size_t m_position{0};
};
```

#### Best Practices

**Use `auto` judiciously**: When type is obvious from context or when dealing with complex template types.

```cpp
// Good
auto json = JsonParser::parse(data);
auto it = container.begin();

// Avoid when clarity suffers
auto x = 42; // Better: int x = 42;
```

**Prefer `std::string_view` over `const std::string&`** for read-only string parameters.

**Use `std::expected` or `std::optional`** for error handling instead of exceptions in hot paths.

**Const correctness**: Mark methods `const` when they don't modify state.

**Use `= default` and `= delete`**: Be explicit about special member functions.

```cpp
class JsonValue {
public:
    JsonValue() = default;
    JsonValue(const JsonValue&) = default;
    JsonValue(JsonValue&&) noexcept = default;
    auto operator=(const JsonValue&) -> JsonValue& = default;
    auto operator=(JsonValue&&) noexcept -> JsonValue& = default;
    ~JsonValue() = default;
};
```

**Avoid implicit conversions**: Use `explicit` for single-argument constructors.

**Header guards**: Use `#pragma once` for simplicity.

### Error Handling

- Use exceptions for truly exceptional cases (e.g., out of memory)
- Use `std::expected` for expected error conditions (e.g., parse failures)
- Document all exceptions a function might throw
- Never throw from destructors or `noexcept` functions

### Documentation

- Use Doxygen-style comments for public APIs
- Explain *why*, not just *what*
- Document preconditions, postconditions, and invariants

```cpp
/**
 * @brief Parses a JSON string into a JsonValue object
 * 
 * @param input The JSON string to parse (must be valid UTF-8)
 * @return Expected JsonValue on success, ParseError on failure
 * 
 * @note This function is thread-safe
 * @throws std::bad_alloc if memory allocation fails
 */
auto parse(std::string_view input) -> std::expected<JsonValue, ParseError>;
```

## Testing Guidelines

- Write unit tests for all new functionality
- Use descriptive test names: `test_parse_empty_object_succeeds()`
- Aim for high code coverage (>80%)
- Test edge cases and error paths

## Questions?

If you have questions, please open an issue labeled "question" or reach out to the maintainers.