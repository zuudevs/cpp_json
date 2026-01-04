# API Reference

Complete API documentation for Cpp Json library.

## Namespace

All classes and functions are in the `cpp_json` namespace:

```cpp
namespace cpp_json {
	// All API classes here
}
```

## Core Classes

### JsonValue

Main class representing a JSON value of any type.

```cpp
class JsonValue {
public:
	// Constructors
	JsonValue();                              // null
	explicit JsonValue(bool value);           // boolean
	explicit JsonValue(double value);         // number
	explicit JsonValue(std::string value);    // string
	explicit JsonValue(const char* value);    // string
	explicit JsonValue(JsonArray value);      // array
	explicit JsonValue(JsonObject value);     // object
	
	// Copy and move
	JsonValue(const JsonValue&);
	JsonValue(JsonValue&&) noexcept;
	auto operator=(const JsonValue&) -> JsonValue&;
	auto operator=(JsonValue&&) noexcept -> JsonValue&;
	
	// Type checking
	auto is_null() const -> bool;
	auto is_bool() const -> bool;
	auto is_number() const -> bool;
	auto is_string() const -> bool;
	auto is_array() const -> bool;
	auto is_object() const -> bool;
	
	auto type() const -> ValueType;
	
	// Value access (throws std::bad_variant_access if wrong type)
	auto as_bool() const -> bool;
	auto as_number() const -> double;
	auto as_string() const -> std::string_view;
	auto as_array() const -> const JsonArray&;
	auto as_array() -> JsonArray&;
	auto as_object() const -> const JsonObject&;
	auto as_object() -> JsonObject&;
	
	// Comparison
	auto operator==(const JsonValue&) const -> bool;
	auto operator!=(const JsonValue&) const -> bool;
};
```

**ValueType Enum:**

```cpp
enum class ValueType {
	Null,
	Boolean,
	Number,
	String,
	Array,
	Object
};
```

### JsonObject

Represents a JSON object (key-value pairs).

```cpp
class JsonObject {
public:
	using Container = std::unordered_map<std::string, JsonValue>;
	using iterator = Container::iterator;
	using const_iterator = Container::const_iterator;
	
	// Constructors
	JsonObject() = default;
	explicit JsonObject(std::initializer_list<std::pair<std::string, JsonValue>>);
	
	// Element access
	auto operator[](std::string_view key) -> JsonValue&;
	auto at(std::string_view key) -> JsonValue&;
	auto at(std::string_view key) const -> const JsonValue&;
	
	// Lookup
	auto contains(std::string_view key) const -> bool;
	auto find(std::string_view key) -> iterator;
	auto find(std::string_view key) const -> const_iterator;
	
	// Capacity
	auto empty() const -> bool;
	auto size() const -> size_t;
	
	// Modifiers
	auto insert(std::string key, JsonValue value) -> std::pair<iterator, bool>;
	auto insert_or_assign(std::string key, JsonValue value) -> std::pair<iterator, bool>;
	auto erase(std::string_view key) -> size_t;
	auto clear() -> void;
	
	// Iterators
	auto begin() -> iterator;
	auto end() -> iterator;
	auto begin() const -> const_iterator;
	auto end() const -> const_iterator;
	auto cbegin() const -> const_iterator;
	auto cend() const -> const_iterator;
};
```

### JsonArray

Represents a JSON array.

```cpp
class JsonArray {
public:
	using Container = std::vector<JsonValue>;
	using iterator = Container::iterator;
	using const_iterator = Container::const_iterator;
	
	// Constructors
	JsonArray() = default;
	explicit JsonArray(std::initializer_list<JsonValue>);
	
	// Element access
	auto operator[](size_t index) -> JsonValue&;
	auto at(size_t index) -> JsonValue&;
	auto at(size_t index) const -> const JsonValue&;
	auto front() -> JsonValue&;
	auto back() -> JsonValue&;
	
	// Capacity
	auto empty() const -> bool;
	auto size() const -> size_t;
	auto reserve(size_t capacity) -> void;
	
	// Modifiers
	auto push_back(JsonValue value) -> void;
	auto emplace_back(auto&&... args) -> JsonValue&;
	auto pop_back() -> void;
	auto insert(const_iterator pos, JsonValue value) -> iterator;
	auto erase(const_iterator pos) -> iterator;
	auto clear() -> void;
	
	// Iterators
	auto begin() -> iterator;
	auto end() -> iterator;
	auto begin() const -> const_iterator;
	auto end() const -> const_iterator;
	auto cbegin() const -> const_iterator;
	auto cend() const -> const_iterator;
};
```

## Parsing

### JsonParser

Static class for parsing JSON strings.

```cpp
class JsonParser {
public:
	/**
	 * Parse a JSON string into a JsonValue
	 * 
	 * @param input UTF-8 encoded JSON string
	 * @return Expected JsonValue on success, ParseError on failure
	 */
	static auto parse(std::string_view input) 
		-> std::expected<JsonValue, ParseError>;
	
	/**
	 * Parse a JSON string with custom options
	 * 
	 * @param input UTF-8 encoded JSON string
	 * @param options Parsing options
	 * @return Expected JsonValue on success, ParseError on failure
	 */
	static auto parse(std::string_view input, const ParseOptions& options) 
		-> std::expected<JsonValue, ParseError>;
};
```

### ParseError

Error information when parsing fails.

```cpp
struct ParseError {
	std::string message;    // Human-readable error message
	size_t line;           // Line number (1-indexed)
	size_t column;         // Column number (1-indexed)
	size_t position;       // Character position in input (0-indexed)
	
	// Convert to string for display
	auto to_string() const -> std::string;
};
```

### ParseOptions

Configuration options for parsing.

```cpp
struct ParseOptions {
	size_t max_depth = 512;              // Maximum nesting depth
	bool allow_comments = false;         // Allow // and /* */ comments
	bool allow_trailing_commas = false;  // Allow trailing commas in arrays/objects
	bool allow_nan_and_inf = false;      // Allow NaN and Infinity values
};
```

## Serialization

### JsonSerializer

Static class for serializing JsonValue to JSON strings.

```cpp
class JsonSerializer {
public:
	/**
	 * Serialize a JsonValue to a compact JSON string
	 * 
	 * @param value The JsonValue to serialize
	 * @return JSON string (UTF-8 encoded)
	 */
	static auto serialize(const JsonValue& value) -> std::string;
	
	/**
	 * Serialize a JsonValue to a pretty-printed JSON string
	 * 
	 * @param value The JsonValue to serialize
	 * @param indent_size Number of spaces per indentation level
	 * @return Pretty-printed JSON string (UTF-8 encoded)
	 */
	static auto serialize_pretty(const JsonValue& value, size_t indent_size = 2) 
		-> std::string;
	
	/**
	 * Serialize with custom options
	 * 
	 * @param value The JsonValue to serialize
	 * @param options Serialization options
	 * @return JSON string (UTF-8 encoded)
	 */
	static auto serialize(const JsonValue& value, const SerializeOptions& options) 
		-> std::string;
};
```

### SerializeOptions

Configuration options for serialization.

```cpp
struct SerializeOptions {
	bool pretty_print = false;           // Enable pretty printing
	size_t indent_size = 2;              // Spaces per indent level
	bool escape_unicode = false;         // Escape non-ASCII as \uXXXX
	bool ensure_ascii = false;           // Ensure all output is ASCII
	size_t float_precision = 6;          // Decimal places for floats
};
```

## Utility Functions

### Type Conversion Helpers

```cpp
namespace cpp_json {
	// Convert JsonValue to native types with optional default
	template <typename T>
	auto get_or(const JsonValue& value, T default_value) -> T;
	
	// Try to convert, returns std::optional
	template <typename T>
	auto try_get(const JsonValue& value) -> std::optional<T>;
}
```

**Example:**

```cpp
auto name = get_or(json["name"], std::string("Unknown"));
auto age = try_get<int>(json["age"]); // std::optional<int>
```

## Error Handling

All operations that can fail return `std::expected<T, E>`:

```cpp
auto result = JsonParser::parse(input);

if (result) {
	// Success path
	JsonValue& json = result.value();
	// Use json...
} else {
	// Error path
	const ParseError& error = result.error();
	std::cerr << error.to_string() << "\n";
}
```

## Exception Guarantee

- **No-throw guarantee**: Move constructors and move assignments
- **Strong exception safety**: All modifying operations
- **Basic exception safety**: Parsing and serialization

The library only throws exceptions for:
- `std::bad_alloc` (out of memory)
- `std::bad_variant_access` (wrong type access on JsonValue)
- `std::out_of_range` (invalid index/key with `at()`)

## Thread Safety

- **Const operations**: Thread-safe (multiple readers)
- **Non-const operations**: Not thread-safe (requires external synchronization)
- **Parsing**: Thread-safe (each parse operation is independent)

## Examples

See [QUICKSTART.md](QUICKSTART.md) for complete examples.

### Parse JSON

```cpp
auto result = JsonParser::parse(R"({"name": "Alice", "age": 30})");
if (result) {
	auto& obj = result.value().as_object();
	std::cout << obj["name"].as_string() << "\n";
}
```

### Build JSON

```cpp
JsonObject person;
person["name"] = JsonValue("Bob");
person["age"] = JsonValue(25.0);

JsonArray hobbies;
hobbies.push_back(JsonValue("Reading"));
hobbies.push_back(JsonValue("Coding"));
person["hobbies"] = JsonValue(std::move(hobbies));

std::string json_str = JsonSerializer::serialize(JsonValue(person));
```

### Handle Nested Structures

```cpp
if (json.is_object()) {
	auto& obj = json.as_object();
	if (obj.contains("address") && obj["address"].is_object()) {
		auto& address = obj["address"].as_object();
		std::cout << address["city"].as_string() << "\n";
	}
}
```