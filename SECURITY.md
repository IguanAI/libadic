# Security Policy

## Supported Versions

| Version | Supported          |
| ------- | ------------------ |
| 1.0.x   | :white_check_mark: |
| < 1.0   | :x:                |

## Mathematical Integrity

libadic is a mathematical library where correctness is paramount. We consider the following as critical security issues:

### Critical Issues
- **Mathematical Incorrectness**: Any algorithm producing mathematically wrong results
- **Precision Loss**: Unexpected loss of precision beyond documented bounds
- **Memory Safety**: Buffer overflows, use-after-free, memory leaks
- **Convergence Failures**: Series not converging when they should

### Reporting a Vulnerability

If you discover a security vulnerability or mathematical error, please:

1. **DO NOT** create a public issue
2. Email security@libadic.org with:
   - Description of the issue
   - Steps to reproduce
   - Mathematical proof of incorrectness (if applicable)
   - Suggested fix (if available)
3. Allow up to 48 hours for initial response
4. Work with maintainers to validate and fix the issue

### Security Measures

#### Memory Safety
- All dynamic memory managed through RAII
- No raw pointers in public APIs
- Bounds checking on all array operations
- Regular Valgrind testing in CI

#### Mathematical Validation
- Every algorithm has mathematical proof
- Comprehensive test coverage with known values
- Precision tracking at every step
- Convergence conditions explicitly checked

#### Input Validation
```cpp
// All inputs are validated
if (prime < 2) {
    throw std::invalid_argument("Prime must be >= 2");
}
if (precision < 1) {
    throw std::invalid_argument("Precision must be >= 1");
}
```

#### Cryptographic Note
**libadic is NOT a cryptographic library**. While it implements mathematically rigorous algorithms, it:
- Does not provide constant-time operations
- Is not designed to resist timing attacks
- Should not be used for cryptographic purposes

### Verification Process

All mathematical algorithms undergo:

1. **Formal Review**: Mathematical proof verification
2. **Test Validation**: Known value comparison
3. **Precision Analysis**: Error bound verification
4. **Peer Review**: Community validation

### Bug Bounty

We offer recognition for significant findings:

- **Critical**: Mathematical incorrectness - Public acknowledgment + Co-authorship consideration
- **High**: Memory safety issues - Public acknowledgment
- **Medium**: Performance issues - Contributors list
- **Low**: Documentation errors - Thank you note

### Disclosure Timeline

1. **0-2 days**: Initial response and validation
2. **3-7 days**: Fix development and testing
3. **8-14 days**: Patch release preparation
4. **15 days**: Public disclosure with fix

### Known Limitations

The following are documented limitations, not vulnerabilities:

- Series convergence requires specific conditions (documented)
- Precision is finite and bounded by available memory
- Some operations have exponential complexity for large inputs
- Not all p-adic functions are implemented yet

### Contact

- Security Email: security@libadic.org
- PGP Key: [Available on request]
- Response Time: 48 hours maximum

### Compliance

This library adheres to:
- ISO/IEC 9899:2011 (C11) safety guidelines
- IEEE 754 floating-point standards where applicable
- Mathematical software verification standards

---

*"The pursuit of mathematical truth requires absolute integrity."*