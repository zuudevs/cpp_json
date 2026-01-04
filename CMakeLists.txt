cmake_minimum_required(VERSION 3.20)

# Fix CMake Warning CMP0135 (Download Timestamp)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(cpp_json
    VERSION 2.1.0
    DESCRIPTION "Modern C++23 JSON parser"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. Library Definition ---
add_library(${PROJECT_NAME}
    src/tokenizer.cpp
    src/parser.cpp
    src/writer.cpp
    src/api.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/include")

# Compiler Flags (Strict for YOUR code only)
if(MSVC OR CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
    # /W4 = High warning level
    # /wd4996 = Ignore deprecated warnings
    # /wd4251 = Ignore dll-interface warnings
    target_compile_options(${PROJECT_NAME} PRIVATE /O2 /W4 /wd4996 /wd4251)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -O3 -Wall -Wextra -Wno-c++98-compat)
endif()

# --- 2. Unified Tool ---
add_executable(json-tool tools/json_tool.cpp)
target_link_libraries(json-tool PRIVATE ${PROJECT_NAME})

# Helper to suppress warnings for external targets
function(disable_target_warnings target)
    if(TARGET ${target})
        if(MSVC OR CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
            target_compile_options(${target} PRIVATE /w)
        else()
            target_compile_options(${target} PRIVATE -w)
        endif()
    endif()
endfunction()

# --- 3. Google Test Integration ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE # Fix CMake warning
)
# Force shared CRT to match project
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# MUTE GTest Warnings (Penting!)
disable_target_warnings(gtest)
disable_target_warnings(gtest_main)
disable_target_warnings(gmock)
disable_target_warnings(gmock_main)

# --- 4. Unit Tests ---
enable_testing()

add_executable(unit_tests tests/unit_tests.cpp)
target_link_libraries(unit_tests PRIVATE ${PROJECT_NAME} GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)