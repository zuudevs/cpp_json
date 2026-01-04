/**
 * @file cpp_json.hpp
 * @author zuudevs (zuudevs@gmail.com)
 * @brief Main header for cpp_json library.
 * 
 * This is the single include file that brings in the entire public API
 * of the cpp_json library, including parsing, writing, and building JSON.
 * It also provides library metadata functions.
 * @version 1.0.0
 * @date 2026-01-04
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "json/api.hpp"
#include "json/builder.hpp"

namespace json {

/**
 * @brief Returns the full version string of the library.
 * 
 * @return std::string_view The version string (e.g., "1.1.0").
 */
inline consteval std::string_view version() noexcept {
    return "1.1.0";
}

/**
 * @brief Returns the major version number.
 * 
 * @return std::string_view The major version.
 */
inline consteval std::string_view major() noexcept {
	return "1";
}

/**
 * @brief Returns the minor version number.
 * 
 * @return std::string_view The minor version.
 */
inline consteval std::string_view minor() noexcept {
	return "1";
}

/**
 * @brief Returns the patch version number.
 * 
 * @return std::string_view The patch version.
 */
inline consteval std::string_view patch() noexcept {
	return "0";
}

/**
 * @brief Returns the author of the library.
 * 
 * @return std::string_view The author's name.
 */
inline consteval std::string_view author() noexcept {
	return "zuudevs";
}

/**
 * @brief Returns the author's email address.
 * 
 * @return std::string_view The email address.
 */
inline consteval std::string_view email() noexcept {
	return "zuudevs@gmail.com";
}

/**
 * @brief Returns the license type of the library.
 * 
 * @return std::string_view The license name (e.g., "MIT").
 */
inline consteval std::string_view license() noexcept {
	return "MIT";
}

} // namespace json
