#include "json/writer.hpp"
#include <format>

namespace json {

std::string Writer::write(const Node* root) {
    std::string result;
    result.reserve(1024);
    write_node(root, result);
    return result;
}

void Writer::write(const Node* root, FILE* out) {
    std::string str = write(root);
    std::fwrite(str.data(), 1, str.size(), out);
}

void Writer::write_node(const Node* node, std::string& out) {
    if (!node) {
        out += "null";
        return;
    }

    switch (node->type) {
        case NodeType::Null:
            out += "null";
            break;

        case NodeType::Bool:
            out += node->bool_val ? "true" : "false";
            break;

        case NodeType::Number:
            out += std::format("{}", node->number_val);
            break;

        case NodeType::String:
            write_string(node->string_val.view(), out);
            break;

        case NodeType::Array: {
            out += '[';
            if (pretty_ && node->array_val.size > 0) {
                write_newline(out);
                ++indent_;
            }

            for (size_t i = 0; i < node->array_val.size; ++i) {
                if (pretty_) write_indent(out);
                write_node(node->array_val.data[i], out);
                if (i + 1 < node->array_val.size) {
                    out += ',';
                }
                if (pretty_) write_newline(out);
            }

            if (pretty_ && node->array_val.size > 0) {
                --indent_;
                write_indent(out);
            }
            out += ']';
            break;
        }

        case NodeType::Object: {
            out += '{';
            if (pretty_ && node->object_val.size > 0) {
                write_newline(out);
                ++indent_;
            }

            for (size_t i = 0; i < node->object_val.size; ++i) {
                if (pretty_) write_indent(out);
                
                write_string(node->object_val.data[i].key.view(), out);
                out += pretty_ ? ": " : ":";
                write_node(node->object_val.data[i].value, out);
                
                if (i + 1 < node->object_val.size) {
                    out += ',';
                }
                if (pretty_) write_newline(out);
            }

            if (pretty_ && node->object_val.size > 0) {
                --indent_;
                write_indent(out);
            }
            out += '}';
            break;
        }
    }
}

void Writer::write_string(std::string_view str, std::string& out) {
    out += '"';
    for (char c : str) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (c >= 0x20) {
                    out += c;
                } else {
                    out += std::format("\\u{:04x}", static_cast<unsigned char>(c));
                }
        }
    }
    out += '"';
}

void Writer::write_indent(std::string& out) {
    out.append(indent_ * indent_size_, ' ');
}

void Writer::write_newline(std::string& out) {
    out += '\n';
}

} // namespace json