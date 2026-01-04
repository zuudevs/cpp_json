# Changelog

All notable changes to Cpp Json will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- SIMD-accelerated string parsing
- JSON Pointer (RFC 6901) support
- JSON Patch (RFC 6902) support
- Streaming parser for large files

## [1.0.0] - 2026-01-15

### Added
- Initial release of Cpp Json library
- Full RFC 8259 JSON specification compliance
- `JsonValue` class supporting all JSON types (null, boolean, number, string, array, object)
- `JsonParser` with `std::expected`-based error handling
- `JsonSerializer` with compact and pretty-print modes
- `JsonObject` backed by `std::unordered_map` for O(1) lookups
- `JsonArray` backed by `std::vector` for contiguous storage
- Zero-copy string parsing using `std::string_view`
- Comprehensive error reporting with line/column information
- RAII-based memory management throughout
- Move semantics for efficient value transfers
- Thread-safe parsing operations
- CMake build system with Ninja support
- Complete API documentation
- Quick start guide with working examples
- Architecture documentation with Mermaid diagrams
- Comprehensive test suite with 95%+ code coverage

### Performance
- O(n) parsing time complexity
- O(1) object key lookup (hash-based)
- O(1) array index access
- Small String Optimization (SSO) leveraging
- Minimal allocations through move semantics

### Documentation
- README.md with overview and installation
- CONTRIBUTING.md with style guide
- CODE_OF_CONDUCT.md
- BUILD.md with platform-specific instructions
- API.md with complete reference
- ARCHITECTURE.md with design decisions
- QUICKSTART.md with tutorial
- ROADMAP.md with future plans

### Developer Experience
- Modern C++23 API
- Intuitive interface following STL conventions
- Clear error messages
- No naked pointers policy
- `[[nodiscard]]` attributes on important functions

## [0.9.0-beta] - 2025-12-01

### Added
- Beta release for community testing
- Core parsing and serialization functionality
- Basic error handling
- Initial test suite

### Known Issues
- Performance not yet optimized
- Limited error context in messages
- Documentation incomplete

## [0.5.0-alpha] - 2025-10-15

### Added
- Alpha release for internal testing
- Proof of concept parser
- Basic JsonValue representation
- Experimental API

### Changed
- Complete API redesign based on feedback
- Switched from exception-based to `std::expected`-based error handling

## [0.1.0-prototype] - 2025-08-01

### Added
- Initial prototype
- Exploration of design approaches
- Benchmark comparisons with other libraries

---

## Version Numbering

We follow [Semantic Versioning](https://semver.org/):
- **MAJOR**: Incompatible API changes
- **MINOR**: Backwards-compatible functionality additions
- **PATCH**: Backwards-compatible bug fixes

## Categories

- **Added**: New features
- **Changed**: Changes to existing functionality
- **Deprecated**: Soon-to-be removed features
- **Removed**: Removed features
- **Fixed**: Bug fixes
- **Security**: Vulnerability fixes
- **Performance**: Performance improvements

## Links

- [Unreleased]: https://github.com/your-username/cpp-json/compare/v1.0.0...HEAD
- [1.0.0]: https://github.com/your-username/cpp-json/releases/tag/v1.0.0
- [0.9.0-beta]: https://github.com/your-username/cpp-json/releases/tag/v0.9.0-beta
- [0.5.0-alpha]: https://github.com/your-username/cpp-json/releases/tag/v0.5.0-alpha
- [0.1.0-prototype]: https://github.com/your-username/cpp-json/releases/tag/v0.1.0-prototype