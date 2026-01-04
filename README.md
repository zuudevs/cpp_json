# cpp_json

![Language](https://img.shields.io/badge/language-C%2B%2B23-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)
![Version](https://img.shields.io/badge/version-1.1.0-orange.svg)

**High-performance, zero-copy, arena-based JSON parser for Modern C++23.**

`cpp_json` is designed for systems where performance, predictability, and memory control are paramount. It avoids hidden allocations, exceptions, and runtime overhead, making it ideal for game engines, embedded systems, and high-throughput services.

## ✨ Key Features

* 🚀 **Performance First**: Zero-copy parsing using `std::string_view` where possible.
* 🧠 **Arena Allocation**: Zero fragmentation, predictable memory usage, and instant cleanup.
* 🛡️ **Safety**: No exceptions (`std::expected`), no RTTI, and strict bounds checking.
* 🏗️ **Builder API**: Fluent, type-safe API for constructing JSON programmatically.
* 🔧 **Modern C++**: Built strictly on C++23 standards (Iterators, Ranges, Concepts).

## 📦 Quick Install

### CMake FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
  cpp_json
  GIT_REPOSITORY [https://github.com/zuudevs/cpp_json.git](https://github.com/zuudevs/cpp_json.git)
  GIT_TAG v1.1.0
)
FetchContent_MakeAvailable(cpp_json)
target_link_libraries(your_target PRIVATE cpp_json)

```

## 📜 License

Distributed under the MIT License. See [LICENSE](https://www.google.com/search?q=LICENSE) for more information.