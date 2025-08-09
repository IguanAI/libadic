# Contributing to libadic

First off, thank you for considering contributing to libadic! It's people like you that make libadic a groundbreaking tool for mathematical research.

## 🎯 Vision

libadic aims to be the definitive implementation of p-adic arithmetic for validating the Reid-Li Criterion. We value:

- **Mathematical Rigor**: Every algorithm must be mathematically correct
- **Performance**: Optimized for large-scale computations
- **Clarity**: Code should be self-documenting with clear intent
- **Testing**: Comprehensive validation with mathematical proofs

## 🚀 Getting Started

### Prerequisites

1. Fork the repository on GitHub
2. Clone your fork locally:
   ```bash
   git clone https://github.com/yourusername/libadic.git
   cd libadic
   ```
3. Add the upstream repository:
   ```bash
   git remote add upstream https://github.com/originalowner/libadic.git
   ```

### Development Environment

We recommend using the Docker environment for consistency:

```bash
docker-compose up -d
docker-compose exec libadic bash
```

## 🔧 Development Process

### 1. Create a Branch

Create a feature branch from `main`:

```bash
git checkout -b feature/your-feature-name
```

Use descriptive branch names:
- `feature/` - New features
- `fix/` - Bug fixes
- `docs/` - Documentation improvements
- `perf/` - Performance improvements
- `test/` - Test improvements

### 2. Make Your Changes

#### Code Style Guidelines

- **C++ Standard**: Use C++17 features appropriately
- **Naming**:
  - Classes: `PascalCase`
  - Functions/Methods: `snake_case`
  - Constants: `UPPER_SNAKE_CASE`
  - Member variables: `member_var_` (trailing underscore for private)
- **Headers**: Use include guards, not `#pragma once`
- **Memory**: RAII everywhere - no manual memory management
- **Errors**: Use exceptions for error handling

#### Mathematical Requirements

Every mathematical function must include:

1. **Convergence conditions** clearly documented
2. **Precision guarantees** explicitly stated
3. **Mathematical proof** or reference to literature
4. **Test cases** validating the mathematics

Example:
```cpp
/**
 * Computes the p-adic logarithm of x.
 * 
 * Mathematical basis: Taylor series log(1+u) = u - u²/2 + u³/3 - ...
 * Convergence: Requires x ≡ 1 (mod p) for p ≠ 2, or x ≡ 1 (mod 4) for p = 2
 * Precision: Returns result with precision O(p^N) where N is input precision
 * 
 * Reference: Koblitz, "p-adic Numbers", Chapter 4
 * 
 * @param x p-adic number with valuation 0
 * @return p-adic logarithm of x
 * @throws domain_error if convergence conditions not met
 */
Qp log_p(const Qp& x);
```

### 3. Write Tests

Every contribution must include tests:

```cpp
void test_my_feature() {
    TestFramework test("My Feature Test");
    
    // Test the mathematical identity
    test.mathematical_proof(
        "Identity name",
        "Proof description",
        actual_value == expected_value
    );
    
    // Test edge cases
    test.assert_true(edge_case_handled, "Edge case description");
    
    // Validate precision
    test.assert_precision(computed, expected, tolerance, "Precision test");
    
    test.report();
    test.require_all_passed();
}
```

### 4. Update Documentation

- Update the README if you've added new features
- Add inline documentation for all public APIs
- Include mathematical proofs in comments
- Update CHANGELOG.md with your changes

### 5. Commit Your Changes

Write clear, descriptive commit messages:

```bash
git commit -m "feat: Add Iwasawa logarithm for p-adic L-functions

- Implement convergent series expansion
- Add Mahler coefficient computation
- Validate against Coleman's formula
- Test convergence for p=2,3,5,7"
```

Follow conventional commits:
- `feat:` - New feature
- `fix:` - Bug fix
- `docs:` - Documentation
- `perf:` - Performance improvement
- `test:` - Test addition/modification
- `refactor:` - Code refactoring

### 6. Run Tests

Before pushing, ensure all tests pass:

```bash
cd build
cmake ..
make -j$(nproc)
ctest --verbose
```

Verify mathematical correctness:
```bash
./milestone1_test 5 60
./milestone1_test 7 60
./milestone1_test 11 60
```

### 7. Push and Create PR

```bash
git push origin feature/your-feature-name
```

Then create a Pull Request on GitHub with:
- Clear description of changes
- Mathematical justification if applicable
- Test results
- Any benchmark improvements

## 📊 Performance Contributions

When optimizing performance:

1. **Benchmark First**: Measure before and after
2. **Document Complexity**: State big-O complexity
3. **Maintain Accuracy**: Never sacrifice correctness for speed
4. **Test Thoroughly**: Include large-precision tests

Example benchmark format:
```cpp
// Benchmark: Computing log_p for 1000-digit precision
// Before: 2.3s
// After: 0.8s
// Method: Implemented binary splitting for series evaluation
```

## 🔬 Mathematical Contributions

For new mathematical algorithms:

1. **Provide Proof**: Include complete mathematical proof or citation
2. **Verify Convergence**: Document convergence conditions
3. **Test Exhaustively**: Multiple test cases across different primes
4. **Compare Literature**: Reference existing implementations

## 🐛 Reporting Issues

### Bug Reports

Include:
- Minimal reproducible example
- Expected behavior
- Actual behavior
- System information (OS, compiler, GMP version)
- Mathematical context if relevant

### Feature Requests

Include:
- Mathematical motivation
- Use case description
- Proposed implementation approach
- References to literature

## 📚 Resources

### Mathematical References

- Koblitz, N. "p-adic Numbers, p-adic Analysis, and Zeta-Functions"
- Robert, A. "A Course in p-adic Analysis"
- Gouvêa, F. "p-adic Numbers: An Introduction"

### C++ Resources

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- [GMP Manual](https://gmplib.org/manual/)
- [Modern C++ Best Practices](https://github.com/cpp-best-practices/cppbestpractices)

## 🎖️ Recognition

Contributors will be:
- Listed in CONTRIBUTORS.md
- Acknowledged in release notes
- Credited in academic publications using the library

## 💬 Communication

- **GitHub Issues**: Technical discussions
- **GitHub Discussions**: General questions and ideas
- **Pull Requests**: Code review and implementation discussion

## ⚖️ Code of Conduct

### Our Standards

- Be respectful and inclusive
- Welcome newcomers and help them get started
- Focus on mathematical correctness and clarity
- Accept constructive criticism gracefully
- Put the project's goals first

### Mathematical Integrity

- Never compromise mathematical correctness
- Always verify claims with proofs or citations
- Acknowledge limitations and assumptions
- Be transparent about precision and error bounds

## 📄 License

By contributing, you agree that your contributions will be licensed under the MIT License.

## 🙏 Thank You!

Your contributions are helping advance mathematical research on one of the most important unsolved problems in mathematics. Every improvement, no matter how small, brings us closer to validating the Reid-Li Criterion.

---

*"Pure mathematics is, in its way, the poetry of logical ideas."* - Albert Einstein