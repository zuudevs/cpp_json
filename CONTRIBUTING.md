# Contributing to cpp_json

Thank you for your interest in contributing! We love high-performance C++ and clean code.

## Development Workflow

1.  Fork the repository.
2.  Create a new branch (`git checkout -b feature/amazing-feature`).
3.  Commit your changes following the [Conventional Commits](https://www.conventionalcommits.org/) specification.
4.  Push to the branch.
5.  Open a Pull Request.

## Coding Style

* **Standard**: C++23.
* **Format**: We use `.clang-format`. Please run it before committing.
* **Allocations**: **NEVER** use `new`/`delete` or `std::shared_ptr` inside the hot path. Use the `Arena`.
* **Exceptions**: Do not use exceptions. Use `std::expected` for error handling.
* **RTTI**: Do not use `dynamic_cast`.

## Testing

Ensure all tests pass before submitting a PR. Add new tests for new features in `tests/`.