#pragma once

#include "ast.hpp"
#include "arena.hpp"
#include "error.hpp"
#include <span>
#include <string>

namespace json {

// High-level API
Result<Node*> parse(std::span<const char> input, Arena& arena);
std::string write(const Node* root, bool pretty = false);

} // namespace json