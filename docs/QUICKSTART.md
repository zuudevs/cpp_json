# Quick Start Guide

Get up and running with Cpp Json in 5 minutes.

## Installation

### Using CMake (Recommended)

Add Cpp Json to your project via CMake:

```cmake
# In your CMakeLists.txt
find_package(CppJson REQUIRED)
target_link_libraries(your_target PRIVATE CppJson::CppJson)
```

### Manual Installation

1. Clone the repository
2. Build and install (see [BUILD.md](../BUILD.md))
3. Add include path: `-I/usr/local/include`
4. Link library: `-lCppJson`

## Hello World Example

Here's a complete example that demonstrates parsing and serializing JSON:

```cpp
// main.cpp
#include <cpp_json/json.hpp>
#include <iostream>
#include <expected>

int main() {
	using namespace cpp_json;
	
	// Example 1: Parse a JSON string
	const char* json_str = R"({
		"name": "Alice",
		"age": 30,
		"is_student": false,
		"courses": ["Math", "Physics", "Chemistry"],
		"address": {
			"city": "New York",
			"zip": "10001"
		}
	})";
	
	// Parse the JSON string
	auto result = JsonParser::parse(json_str);
	
	if (!result) {
		std::cerr << "Parse error: " << result.error().message << "\n";
		return 1;
	}
	
	JsonValue& json = result.value();
	
	// Example 2: Access values
	if (json.is_object()) {
		auto& obj = json.as_object();
		
		// Access string
		if (obj.contains("name")) {
			std::cout << "Name: " << obj["name"].as_string() << "\n";
		}
		
		// Access number
		if (obj.contains("age")) {
			std::cout << "Age: " << obj["age"].as_number() << "\n";
		}
		
		// Access boolean
		if (obj.contains("is_student")) {
			std::cout << "Student: " << (obj["is_student"].as_bool() ? "Yes" : "No") << "\n";
		}
		
		// Access array
		if (obj.contains("courses") && obj["courses"].is_array()) {
			std::cout << "Courses:\n";
			for (const auto& course : obj["courses"].as_array()) {
				std::cout << "  - " << course.as_string() << "\n";
			}
		}
		
		// Access nested object
		if (obj.contains("address") && obj["address"].is_object()) {
			auto& address = obj["address"].as_object();
			std::cout << "City: " << address["city"].as_string() << "\n";
		}
	}
	
	// Example 3: Build JSON programmatically
	JsonObject person;
	person["name"] = JsonValue("Bob");
	person["age"] = JsonValue(25.0);
	person["employed"] = JsonValue(true);
	
	JsonArray hobbies;
	hobbies.push_back(JsonValue("Reading"));
	hobbies.push_back(JsonValue("Gaming"));
	hobbies.push_back(JsonValue("Hiking"));
	person["hobbies"] = JsonValue(std::move(hobbies));
	
	JsonValue person_json(std::move(person));
	
	// Example 4: Serialize to JSON string
	std::string serialized = JsonSerializer::serialize(person_json);
	std::cout << "\nSerialized JSON:\n" << serialized << "\n";
	
	// Pretty print with indentation
	std::string pretty = JsonSerializer::serialize_pretty(person_json, 2);
	std::cout << "\nPretty printed:\n" << pretty << "\n";
	
	return 0;
}
```

## Compiling the Example

### Using g++ directly

```bash
g++ -std=c++23 -O2 main.cpp -o json_example -lCppJson
./json_example
```

### Using CMake

Create a `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.25)
project(JsonExample CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CppJson REQUIRED)

add_executable(json_example main.cpp)
target_link_libraries(json_example PRIVATE CppJson::CppJson)
```

Build and run:

```bash
mkdir build && cd build
cmake ..
make
./json_example
```

## Expected Output

```
Name: Alice
Age: 30
Student: No
Courses:
  - Math
  - Physics
  - Chemistry
City: New York

Serialized JSON:
{"name":"Bob","age":25.0,"employed":true,"hobbies":["Reading","Gaming","Hiking"]}

Pretty printed:
{
  "name": "Bob",
  "age": 25.0,
  "employed": true,
  "hobbies": [
	"Reading",
	"Gaming",
	"Hiking"
  ]
}
```

## Common Operations

### Checking Value Types

```cpp
if (value.is_object()) { /* ... */ }
if (value.is_array()) { /* ... */ }
if (value.is_string()) { /* ... */ }
if (value.is_number()) { /* ... */ }
if (value.is_bool()) { /* ... */ }
if (value.is_null()) { /* ... */ }
```

### Safe Access with Error Handling

```cpp
auto result = JsonParser::parse(json_str);

if (result) {
	// Success
	JsonValue& json = result.value();
	// Use json...
} else {
	// Error
	const ParseError& error = result.error();
	std::cerr << "Parse failed at line " << error.line 
			  << ", column " << error.column 
			  << ": " << error.message << "\n";
}
```

### Iterating Over Objects

```cpp
if (json.is_object()) {
	for (const auto& [key, value] : json.as_object()) {
		std::cout << key << ": ";
		// Process value...
	}
}
```

### Iterating Over Arrays

```cpp
if (json.is_array()) {
	for (size_t i = 0; i < json.as_array().size(); ++i) {
		const auto& element = json.as_array()[i];
		// Process element...
	}
}
```

## Next Steps

- Read the [API Reference](API.md) for detailed documentation
- Explore the [Architecture](ARCHITECTURE.md) to understand the design
- Check out the [examples/](../examples/) directory for more use cases
- Review [CONTRIBUTING.md](../CONTRIBUTING.md) if you want to contribute

## Need Help?

- Open an issue on GitHub
- Check existing issues and discussions
- Read the full documentation in the `docs/` directory