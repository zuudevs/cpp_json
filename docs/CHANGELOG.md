# Changelog

## [1.1.0] - 2026-01-04

### Added
- **String Unescaping**: Full support for JSON escape sequences (`\n`, `\t`, `\uXXXX`).
- **Builder API**: Fluent interface (`json::build_object`) for constructing JSON safely.
- **Iterators**: Added `begin()`/`end()` for C++23 range-based loops support.
- **File I/O**: Added `read_file_to_arena` and `parse_file` helpers.

### Changed
- **API Safety**: Migrated raw accessors to `std::expected` for safer error handling.
- **Tokenizer**: Now requires `Arena` reference for handling escaped strings.

## [1.0.0] - 2025-12-31

- Initial release.
- Basic zero-copy parsing.
- Arena allocator implementation.