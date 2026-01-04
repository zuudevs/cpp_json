#include "json/api.hpp"
#include "json/tokenizer.hpp"
#include "json/parser.hpp"
#include "json/writer.hpp"

namespace json {

Result<Node*> parse(std::span<const char> input, Arena& arena) {
    Tokenizer tokenizer(input);
    Parser parser(arena, tokenizer);
    return parser.parse();
}

std::string write(const Node* root, bool pretty) {
    Writer writer(pretty);
    return writer.write(root);
}

} // namespace json