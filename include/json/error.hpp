/**
 * @file error.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief JSON Error Handling API
 * 
 * This file defines error codes, the Error structure, and the Result type
 * used throughout the JSON library for error reporting.
 * @version 1.0.1
 * @date 2026-01-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include <cstdint>
#include <string_view>
#include <expected>

namespace json {

/**
 * @brief Enumeration of possible error codes during JSON processing.
 */
enum class ErrorCode : uint8_t {
    None = 0,           ///< No error occurred
    UnexpectedEOF,      ///< Unexpected end of file/input
    InvalidToken,       ///< Invalid token encountered
    InvalidNumber,      ///< Malformed number
    InvalidString,      ///< Malformed string
    InvalidEscape,      ///< Invalid escape sequence in string
    UnexpectedToken,    ///< Token found where another was expected
    ExpectedColon,      ///< Expected ':' after object key
    ExpectedComma,      ///< Expected ',' between elements
    ExpectedValue,      ///< Expected a value (null, bool, number, string, array, object)
    TooDeep,            ///< Nesting depth exceeded limit
    OutOfMemory         ///< Memory allocation failed
};

/**
 * @brief Represents an error that occurred during JSON processing.
 * 
 * Contains the error code, the byte offset where the error occurred,
 * and a descriptive message.
 */
struct Error {
    ErrorCode code;             ///< The specific error code
    size_t offset;              ///< Byte offset in the input where error occurred
    std::string_view message;   ///< Human-readable error message

    /**
     * @brief Constructs a new Error object.
     * 
     * @param c The error code.
     * @param off The offset in the input.
     * @param msg The error message.
     */
    constexpr Error(ErrorCode c, size_t off, std::string_view msg) 
        : code(c), offset(off), message(msg) {}
};

/**
 * @brief Converts an ErrorCode to a human-readable string message.
 * 
 * @param code The error code to convert.
 * @return std::string_view The corresponding error message.
 */
constexpr std::string_view error_message(ErrorCode code) {
    switch (code) {
        case ErrorCode::None: return "No error";
        case ErrorCode::UnexpectedEOF: return "Unexpected end of file";
        case ErrorCode::InvalidToken: return "Invalid token";
        case ErrorCode::InvalidNumber: return "Invalid number format";
        case ErrorCode::InvalidString: return "Invalid string";
        case ErrorCode::InvalidEscape: return "Invalid escape sequence";
        case ErrorCode::UnexpectedToken: return "Unexpected token";
        case ErrorCode::ExpectedColon: return "Expected ':'";
        case ErrorCode::ExpectedComma: return "Expected ','";
        case ErrorCode::ExpectedValue: return "Expected value";
        case ErrorCode::TooDeep: return "Nesting too deep";
        case ErrorCode::OutOfMemory: return "Out of memory";
        default: return "Unknown error";
    }
}

/**
 * @brief A standard result type for JSON operations.
 * 
 * Uses std::expected to return either a value of type T or an Error.
 * 
 * @tparam T The type of the success value.
 */
template<typename T>
using Result = std::expected<T, Error>;

} // namespace json
