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
    )
elseif(MSVC)
    add_compile_options(
        /O2
        /W4
        /GR-  # Disable RTTI
    )
endif()

add_library(${PROJECT_NAME}
	src/json/tokenizer.cpp
	src/json/parser.cpp
	src/json/writer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
)

add_executable(json-example tests/basic.cpp)

# Testing
enable_testing()