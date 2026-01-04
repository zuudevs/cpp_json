cmake_minimum_required(VERSION 3.20)
project(cpp_json
    VERSION 1.0.0
    DESCRIPTION "json i/o based on cpp"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -O3
        -Wall
        -Wextra
        -fno-exceptions
        -fno-rtti
        -Wno-c++98-compat  # Suppress C++98 warnings
    )
elseif(MSVC)
    add_compile_options(
        /O2
        /W4
        /GR-  # Disable RTTI
        /wd4996  # Disable deprecated warnings
    )
endif()

# Build the library
add_library(${PROJECT_NAME}
    src/json/tokenizer.cpp
    src/json/parser.cpp
    src/json/writer.cpp
    src/json/api.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
)

# Build the test executable
add_executable(json-example tests/basic.cpp)
target_link_libraries(json-example ${PROJECT_NAME})

# Build json-reader tool
add_executable(json-reader tools/json-reader.cpp)
target_link_libraries(json-reader ${PROJECT_NAME})

# Build json-writer tool
add_executable(json-writer tools/json-writer.cpp)
target_link_libraries(json-writer ${PROJECT_NAME})

# Testing
enable_testing()
add_test(NAME BasicTest COMMAND json-example)