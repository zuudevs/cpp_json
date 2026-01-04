# Architecture Design

## Core Principles

1.  **Memory Arena**: `cpp_json` uses a monotonic allocator (`Arena`). Memory is allocated in large blocks (default 64KB). Individual nodes are allocated linearly. Deallocation happens all at once when the `Arena` is destroyed. This eliminates `malloc`/`free` overhead per node.
2.  **Zero-Copy Strings**: By default, the parser uses `std::string_view` pointing to the original input buffer. New memory is allocated *only* if string unescaping (e.g., `\n`, `\uXXXX`) is required.
3.  **Recursive Descent**: The parser uses a standard recursive descent algorithm but is protected against stack overflow via a strict depth limit (`MaxDepth = 256`).

## Class Overview

* **`Node`**: A trivial, aggregate type (POD) containing a union of values. No virtual table.
* **`Arena`**: Manages memory blocks.
* **`Tokenizer`**: Scans the input buffer. Handles UTF-8 decoding and unescaping.
* **`Parser`**: Constructs the AST (Abstract Syntax Tree) from tokens.