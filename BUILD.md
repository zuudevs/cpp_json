# Building Cpp Json

This guide provides step-by-step instructions for building Cpp Json from source.

## Prerequisites

### Required Tools

- **C++23 Compiler**:
  - GCC 13+ (Linux)
  - Clang 16+ (Linux/macOS)
  - MSVC 19.34+ (Visual Studio 2022) (Windows)
- **CMake**: Version 3.25 or higher
- **Ninja**: Version 1.11 or higher (recommended) or Make

### Installing Prerequisites

#### Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build
# For GCC 13+
sudo apt install gcc-13 g++-13
```

#### macOS

```bash
# Using Homebrew
brew install cmake ninja llvm
```

#### Windows

1. Install [Visual Studio 2022](https://visualstudio.microsoft.com/) with "Desktop development with C++" workload
2. Install [CMake](https://cmake.org/download/) (choose "Add to PATH" during installation)
3. Install [Ninja](https://github.com/ninja-build/ninja/releases) and add to PATH

## Building the Project

### Quick Build (Unix-like systems)

```bash
# Clone the repository
git clone https://github.com/your-username/cpp-json.git
cd cpp-json

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..

# Build
ninja

# Run tests (optional)
ctest --output-on-failure
```

### Detailed Build Steps

#### 1. Clone the Repository

```bash
git clone https://github.com/your-username/cpp-json.git
cd cpp-json
```

#### 2. Create Build Directory

```bash
mkdir build
cd build
```

Always build out-of-source to keep the source tree clean.

#### 3. Configure with CMake

**Using Ninja (recommended)**:

```bash
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
```

**Using Make**:

```bash
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
```

**Using Visual Studio (Windows)**:

```bash
cmake -G "Visual Studio 17 2022" -A x64 ..
```

#### 4. Build

**With Ninja**:

```bash
ninja
```

**With Make**:

```bash
make -j$(nproc)
```

**With Visual Studio**:

```bash
cmake --build . --config Release
```

#### 5. Run Tests (Optional)

```bash
ctest --output-on-failure
```

Or run tests with verbose output:

```bash
ctest -V
```

## Build Options

### CMake Configuration Options

```bash
# Debug build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..

# Release with debug info
cmake -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo ..

# Enable sanitizers (for development)
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug \
	  -DENABLE_ASAN=ON \
	  -DENABLE_UBSAN=ON ..

# Specify custom compiler
cmake -G Ninja \
	  -DCMAKE_CXX_COMPILER=g++-13 \
	  -DCMAKE_BUILD_TYPE=Release ..
```

### Build Types

- **Debug**: No optimization, full debug symbols (`-O0 -g`)
- **Release**: Full optimization, no debug symbols (`-O3 -DNDEBUG`)
- **RelWithDebInfo**: Optimized with debug symbols (`-O2 -g`)
- **MinSizeRel**: Optimize for size (`-Os -DNDEBUG`)

## Installation

After building, you can install the library system-wide:

```bash
# From the build directory
sudo ninja install

# Or with CMake
sudo cmake --install .
```

Default installation locations:
- Headers: `/usr/local/include/cpp_json/`
- Library: `/usr/local/lib/`
- CMake files: `/usr/local/lib/cmake/CppJson/`

### Custom Install Prefix

```bash
cmake -G Ninja -DCMAKE_INSTALL_PREFIX=/custom/path ..
ninja install
```

## Troubleshooting

### Compiler Not Found

If CMake can't find your compiler:

```bash
export CXX=/usr/bin/g++-13
export CC=/usr/bin/gcc-13
cmake -G Ninja ..
```

### Missing C++23 Support

Ensure your compiler version is up to date:

```bash
g++ --version  # Should be 13+
clang++ --version  # Should be 16+
```

### CMake Version Too Old

```bash
# Ubuntu: Install from Kitware APT repository
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | sudo apt-key add -
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main'
sudo apt update
sudo apt install cmake
```

### Ninja Not Found

If Ninja is not found, use Make instead:

```bash
cmake -G "Unix Makefiles" ..
make
```

## Building Documentation

To build the documentation (requires Doxygen):

```bash
cmake -G Ninja -DBUILD_DOCS=ON ..
ninja docs
```

Documentation will be generated in `build/docs/html/`.

## Performance Build

For maximum performance:

```bash
cmake -G Ninja \
	  -DCMAKE_BUILD_TYPE=Release \
	  -DCMAKE_CXX_FLAGS="-O3 -march=native -flto" \
	  ..
ninja
```

**Warning**: `-march=native` creates non-portable binaries optimized for your CPU.