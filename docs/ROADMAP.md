# Project Roadmap

## Phase 3: Advanced Optimization (Q2 2026)
- [ ] **SIMD Tokenizer**: Utilize AVX2/NEON for faster token scanning.
- [ ] **Small String Optimization**: Inline small strings directly into `Node` struct.

## Phase 4: Extended Features (Q3 2026)
- [ ] **Custom Serializers**: Macros/Templates for struct-to-json mapping.
- [ ] **Streaming Parser**: SAX-style interface for huge files (>1GB).
- [ ] **JSON Pointer**: Support for RFC 6901 (e.g., `/users/0/name`).