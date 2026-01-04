# Cpp Json Roadmap

This document outlines the planned features and improvements for Cpp Json.

## Version 1.0 (Current)

✅ **Core Features**
- [x] RFC 8259 compliant JSON parser
- [x] Support for all JSON types (null, boolean, number, string, array, object)
- [x] `std::expected` based error handling
- [x] RAII-based memory management
- [x] Pretty-printing support
- [x] Zero-copy parsing for strings where possible
- [x] Comprehensive test suite
- [x] Full documentation

## Version 1.1 (Q2 2026)

🚧 **Performance Optimizations**
- [ ] SIMD-accelerated string scanning
- [ ] Memory pool allocator for frequent allocations
- [ ] Benchmark suite comparing against popular JSON libraries
- [ ] Profile-guided optimizations

🚧 **Developer Experience**
- [ ] Better error messages with context snippets
- [ ] JSON schema validation support
- [ ] C++20 concepts for type constraints
- [ ] Clang-format configuration file

## Version 1.2 (Q3 2026)

🔮 **Advanced Features**
- [ ] JSON Pointer (RFC 6901) implementation
  - Navigate JSON documents with path syntax
  - Example: `/users/0/name`
- [ ] JSON Patch (RFC 6902) support
  - Apply transformations to JSON documents
  - Operations: add, remove, replace, move, copy, test
- [ ] JSON Merge Patch (RFC 7386)
  - Simpler partial updates to JSON documents

🔮 **Streaming API**
- [ ] Stream parser for very large JSON files
- [ ] Pull-based parsing interface
- [ ] Memory-mapped file support for efficient large file handling
- [ ] Incremental serialization

## Version 2.0 (Q4 2026)

🌟 **Major Enhancements**
- [ ] JSON Lines (JSONL) format support
- [ ] BSON (Binary JSON) support
- [ ] MessagePack support
- [ ] JSON Schema validator
  - Draft 2020-12 support
  - Custom validators
  - Validation error reporting

🌟 **API Improvements**
- [ ] Fluent builder API for constructing JSON
  ```cpp
  auto json = JsonBuilder()
	  .object()
		  .field("name", "Alice")
		  .field("age", 30)
		  .array("hobbies")
			  .value("Reading")
			  .value("Coding")
		  .end()
	  .end()
	  .build();
  ```
- [ ] Reflection-based serialization/deserialization
  ```cpp
  struct Person {
	  std::string name;
	  int age;
	  std::vector<std::string> hobbies;
  };
  
  auto person = json.as<Person>(); // Automatic conversion
  auto json = JsonValue::from(person); // Automatic serialization
  ```

## Version 2.1 (Q1 2027)

🎯 **Ecosystem Integration**
- [ ] First-class CMake FetchContent support
- [ ] Conan package
- [ ] vcpkg package
- [ ] Package for popular Linux distributions
- [ ] Integration examples with popular frameworks

🎯 **Tooling**
- [ ] JSON formatter CLI tool
- [ ] JSON validator CLI tool
- [ ] JSON diff tool
- [ ] Interactive JSON explorer (TUI)

## Version 3.0 (Future)

💡 **Experimental Features**
- [ ] Compile-time JSON parsing (C++23 constexpr)
- [ ] Custom allocator support for embedded systems
- [ ] WebAssembly bindings
- [ ] Python bindings via pybind11
- [ ] Async parsing with C++20 coroutines

💡 **Advanced Optimizations**
- [ ] Lazy evaluation for large objects
- [ ] Copy-on-write semantics for shared data
- [ ] SIMD-accelerated number parsing
- [ ] Custom UTF-8 validation optimizations

## Community Requests

Features requested by the community (vote on GitHub issues!):
- [ ] JSONPath query language support
- [ ] JSON5 extended syntax support (comments, trailing commas, etc.)
- [ ] CBOR (Concise Binary Object Representation)
- [ ] YAML subset parser
- [ ] TOML support
- [ ] XML to JSON converter

## Long-term Vision

Our long-term goals for Cpp Json:
1. **Best-in-class performance**: Match or exceed the fastest JSON libraries
2. **Developer happiness**: Most intuitive and ergonomic C++ JSON API
3. **Zero compromises**: Safety, performance, and ease-of-use together
4. **Universal adoption**: The go-to JSON library for modern C++ projects

## How to Contribute

Want to help implement these features?

1. Check the [GitHub Issues](https://github.com/your-username/cpp-json/issues) for open tasks
2. Comment on features you're interested in working on
3. See [CONTRIBUTING.md](../CONTRIBUTING.md) for guidelines
4. Submit a PR!

Feature requests are always welcome. Open an issue with the `feature-request` label.

## Version History

### v1.0.0 (January 2026)
- Initial release
- Core JSON parsing and serialization
- Full RFC 8259 compliance
- Modern C++23 API