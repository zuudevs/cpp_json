# Build Instructions

## Prerequisites

* **Compiler**: C++23 compatible compiler
    * GCC 13+
    * Clang 16+
    * MSVC 19.36+ (VS 2022)
* **Build System**: CMake 3.20 or higher

## Building from Source

1.  **Clone the repository**
    ```bash
    git clone [https://github.com/zuudevs/cpp_json.git](https://github.com/zuudevs/cpp_json.git)
    cd cpp_json
    ```

2.  **Configure**
    ```bash
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    ```

3.  **Build**
    ```bash
    cmake --build build --config Release
    ```

## Running Tests

We use CTest for testing.

```bash
cd build
ctest --output-on-failure

```

## Installing

```bash
sudo cmake --install build

```