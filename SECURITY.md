# Security Policy

## Supported Versions

We actively support and provide security updates for the following versions:

| Version | Supported          | End of Support |
| ------- | ------------------ | -------------- |
| 2.1.x   | :white_check_mark: | Current        |
| 2.0.x   | :white_check_mark: | TBD            |
| 1.x.x   | :x:                | 2025-12-31     |

**Note**: Only the latest major version receives security updates. We strongly recommend upgrading to the latest version.

---

## Reporting a Vulnerability

We take security seriously. If you discover a security vulnerability within `cpp_json`, please follow these guidelines:

### **DO NOT** create a public GitHub issue

Security vulnerabilities should **never** be disclosed publicly until they have been addressed. Public disclosure can put users at risk.

### Instead, please:

1. **Email us privately** at: **zuudevs@gmail.com**
   - Use the subject line: `[SECURITY] cpp_json vulnerability report`
   - Include a detailed description of the vulnerability
   - Provide steps to reproduce the issue
   - If possible, include a proof-of-concept (PoC)

2. **Expected Response Time**:
   - Initial acknowledgment: Within **48 hours**
   - Status update: Within **5 business days**
   - Fix timeline: Depends on severity (see below)

3. **What to include in your report**:
   ```
   - Version(s) affected
   - Description of the vulnerability
   - Potential impact (DoS, memory corruption, etc.)
   - Steps to reproduce
   - Proof-of-concept code (if available)
   - Suggested fix (optional)
   ```

---

## Severity Levels & Response Times

| Severity | Description | Response Time |
|----------|-------------|---------------|
| **Critical** | Remote code execution, memory corruption, data exfiltration | Immediate (< 24h) |
| **High** | Denial of service, unauthorized access | 3-7 days |
| **Medium** | Information disclosure, logic errors | 2-4 weeks |
| **Low** | Minor issues with limited impact | Next release |

---

## Disclosure Policy

We follow **Coordinated Vulnerability Disclosure (CVD)**:

1. You report the vulnerability privately
2. We acknowledge receipt within 48 hours
3. We investigate and develop a fix
4. We notify you when the fix is ready
5. We release the patch
6. After the patch is released, we publicly disclose the issue (with credit to you if desired)

**Embargo Period**: We request a minimum of **90 days** before public disclosure to allow users time to update.

---

## Security Best Practices

When using `cpp_json` in your applications:

### ✅ DO:
- **Validate input size** before parsing (prevent DoS via huge files)
- **Set depth limits** if parsing untrusted JSON (default is 256, which is safe)
- **Use arena allocators properly** (call `reset()` or destroy arena after use)
- **Check return values** from `parse()` and accessor methods
- **Keep cpp_json updated** to the latest version

### ❌ DON'T:
- **Don't parse unbounded user input** without size limits
- **Don't expose raw pointers** from the arena outside controlled scopes
- **Don't reuse arenas** across security boundaries without reset
- **Don't ignore error codes** returned by parsing functions

---

## Known Security Considerations

### 1. **Stack Overflow Protection**
`cpp_json` limits recursion depth to 256 levels by default to prevent stack overflow attacks from deeply nested JSON. This is configurable via `Parser::MaxDepth`.

### 2. **Memory Exhaustion**
The arena allocator can consume large amounts of memory if parsing malicious input. Always validate input size before parsing:

```cpp
constexpr size_t MAX_JSON_SIZE = 10 * 1024 * 1024; // 10 MB
if (input.size() > MAX_JSON_SIZE) {
    // Reject oversized input
}
```

### 3. **UTF-8 Validation**
The tokenizer performs basic UTF-8 validation during string unescaping. Malformed UTF-8 will result in parse errors.

### 4. **No Sandbox Execution**
`cpp_json` runs in the same process as your application. Always sanitize and validate JSON from untrusted sources.

---

## Security Audit History

| Date       | Auditor | Type           | Findings | Status   |
|------------|---------|----------------|----------|----------|
| 2026-01-04 | Internal | Code Review   | 0 critical | Complete |
| *TBD*      | External | Penetration Test | - | Planned |

---

## Hall of Fame

We recognize and thank the following security researchers:

*(No submissions yet)*

If you report a valid security issue, you can choose to be listed here with credit.

---

## Questions?

For general security questions (not vulnerabilities), you can:
- Open a public discussion on GitHub
- Email: zuudevs@gmail.com with subject `[SECURITY QUESTION]`

---

**Last Updated**: 2026-01-04  
**Policy Version**: 1.0