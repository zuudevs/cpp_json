#pragma once

#include <cstdint>
#include <string_view>
#include <expected>

namespace json {

enum class ErrorCode : uint8_t {
    None = 0,
    UnexpectedEOF,
    InvalidToken,
    InvalidNumber,
    InvalidString,
    InvalidEscape,
    UnexpectedToken,
    ExpectedColon,
    ExpectedComma,
    ExpectedValue,
    TooDeep,
    OutOfMemory
};

struct Error {
    ErrorCode code;
    size_t offset;
    std::string_view message;

    constexpr Error(ErrorCode c, size_t off, std::string_view msg) 
        : code(c), offset(off), message(msg) {}
};

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

template<typename T>
using Result = std::expected<T, Error>;

} // namespace json
