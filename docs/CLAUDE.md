# CLAUDE.md - libadic Project Context

## 🎯 Project Mission
Building a groundbreaking C++ library for p-adic arithmetic to validate the Reid-Li Criterion - a novel approach to the Riemann Hypothesis. This is NOT a theoretical exercise - it must be production-ready, mathematically rigorous, and absolutely correct.

## ⚠️ Critical User Requirements
- **NO bypasses, bandaids, or workarounds** - Everything must be absolute and correct
- **Exhaustive testing and proofs** - Mathematical accuracy is paramount  
- **Full implementations only** - No placeholders or TODOs
- **Enterprise-grade quality** - Production-ready code and documentation

## 📊 Current Status
- ✅ **Phase 1 Complete**: All core components fully implemented
- ✅ **No placeholders**: Every function has real mathematical implementation
- ✅ **Tests passing**: Basic arithmetic and special functions validated
- 🔄 **Milestone test**: Reid-Li criterion validation for p=5,7,11

## 🏗️ Architecture Overview

```
Application Layer (Reid-Li Calculator)
    ↓
High-Level API (p-adic Gamma, L-functions, Logarithm)  
    ↓
Number Fields (Qp, Zp, Cyclotomic, Characters)
    ↓
Base Arithmetic (GMP Wrapper, Modular Arithmetic)
```

## 📂 Project Structure

```
libadic/
├── include/libadic/        # Public headers (all mathematical interfaces)
├── src/                    # Implementation files
│   ├── base/              # Core arithmetic (GMP wrapper, modular)
│   ├── fields/            # Number fields (Zp, Qp, cyclotomic)
│   └── functions/         # Special functions (gamma, log, L-functions)
├── tests/                 # Comprehensive test suite
├── examples/              # Interactive demo & validation programs
├── scripts/               # Build, test, and demo scripts
├── docs/                  # All documentation
│   └── validation/        # PROOF that libadic is unique (critical!)
└── build/                 # Build output (git-ignored)
```

## 📁 Key Files and Their Purpose

### Core Mathematical Components
- **include/libadic/zp.h**: p-adic integers with precision tracking
- **include/libadic/qp.h**: p-adic numbers (field operations)
- **include/libadic/bernoulli.h**: Bernoulli numbers B_n and B_{n,χ}
- **include/libadic/characters.h**: Dirichlet character enumeration
- **include/libadic/l_functions.h**: Kubota-Leopoldt L-functions
- **include/libadic/padic_gamma.h**: Morita's p-adic Gamma function
- **include/libadic/padic_log.h**: p-adic logarithm

### Critical Test
- **tests/milestone1_test.cpp**: Reid-Li criterion validation
  - Must verify Φ_p^(odd/even)(χ) = Ψ_p^(odd/even)(χ)
  - Target: Pass for p=5,7,11 with precision O(p^60)

## 🔬 Mathematical Formulas Implemented

### Reid-Li Criterion
For odd characters χ:
```
Φ_p^(odd)(χ) = Σ_{a=1}^{p-1} χ(a) * log_p(Γ_p(a))
Ψ_p^(odd)(χ) = L'_p(0, χ)
```

For even characters χ:
```
Φ_p^(even)(χ) = Σ_{a=1}^{p-1} χ(a) * log_p(a/(p-1))
Ψ_p^(even)(χ) = L_p(0, χ)
```

### L-function Formulas
```
L_p(0, χ) = -(1 - χ(p)p^{-1}) * B_{1,χ}
L_p(1-n, χ) = -(1 - χ(p)p^{n-1}) * B_{n,χ}/n
```

### Key Algorithms
- **Hensel Lifting**: Square roots in Zp via Newton's method
- **Teichmüller Characters**: ω(a) = lim a^{p^n}
- **Character Enumeration**: Using generators of (Z/nZ)*
- **Bernoulli Recursion**: Σ_{k=0}^{n} C(n+1,k) B_k = 0

## 🛠️ Build and Test Instructions

### Docker (Recommended)
```bash
docker-compose up -d
docker-compose exec libadic bash
cd build && cmake .. && make -j$(nproc)
ctest --verbose
```

### Local Build
```bash
# Install dependencies
sudo apt-get install libgmp-dev libmpfr-dev

# Build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Run milestone test
./milestone1_test 7 60
```

## 🧪 Testing Philosophy
- Every public method has unit tests
- Mathematical identities verified (Wilson's theorem, Fermat's little theorem)
- Convergence conditions strictly enforced
- No approximations - exact p-adic arithmetic only

## 📝 Implementation Notes

### Complete Implementations (No Placeholders)
- ✅ BernoulliNumbers::bernoulli() - Full recursive implementation
- ✅ BernoulliNumbers::generalized_bernoulli() - B_{n,χ} computation
- ✅ DirichletCharacter::enumerate_primitive_characters() - Complete enumeration
- ✅ LFunctions::kubota_leopoldt() - Full L_p(s,χ) implementation
- ✅ LFunctions::kubota_leopoldt_derivative() - L'_p(s,χ) computation

### Precision Management
- All operations track precision explicitly
- Automatic precision reduction in arithmetic operations
- Valuation tracking for Qp field elements
- Convergence checking for series (log, exp, gamma)

### Performance Optimizations
- Caching for Bernoulli numbers and L-values
- GMP for all bigint operations
- Precomputed character values
- Efficient modular arithmetic

## ⚠️ Critical Areas

### Must Verify
1. Gamma function reflection formula: Γ_p(x)·Γ_p(1-x) = ±1
2. L-function special values match literature
3. Character enumeration captures ALL primitive characters
4. Bernoulli numbers satisfy von Staudt-Clausen theorem

### Known Challenges
- p-adic logarithm convergence for p=2 (needs x ≡ 1 mod 4)
- Fractional Gamma function arguments need distribution relations
- Large precision computations (>100 digits) need optimization

## 📚 References
- **DESIGN.md**: Complete specification document (MUST READ)
- **README.md**: User-facing documentation
- Washington's "Introduction to Cyclotomic Fields"
- Koblitz's "p-adic Numbers, p-adic Analysis, and Zeta-Functions"

## 🎯 Success Criteria
Phase 1 is complete when:
1. milestone1_test passes for p=5,7,11
2. All Reid-Li identities hold to precision O(p^60)
3. No mathematical errors or approximations
4. Clean compilation with -Wall -Wextra -Wpedantic

## 💡 For Future Claude Instances

### Key Context
- User demands **absolute correctness** - no shortcuts
- This validates a potential proof of Riemann Hypothesis
- Every formula must match published mathematics exactly
- Test exhaustively - the mathematics must be bulletproof

### Common Commands
```bash
# Run all tests
ctest --verbose

# Run milestone test
./build/milestone1_test 7 60

# Check specific prime
./build/milestone1_test 11 40

# Debug with assertions
cmake -DCMAKE_BUILD_TYPE=Debug .. && make
```

### If Tests Fail
1. Check convergence conditions for p-adic functions
2. Verify character primitivity testing
3. Ensure Bernoulli number normalization
4. Compare L-function values with literature

### Development Workflow
1. Always check DESIGN.md for specifications
2. Run tests after EVERY change
3. No commits without passing tests
4. Document any deviations from design

## 🚀 Next Steps
- [ ] Optimize for larger primes (p > 100)
- [ ] Parallelize character enumeration
- [ ] Add formal verification with Lean/Coq
- [ ] Implement distributed computation framework
- [ ] Create Python bindings for easier experimentation

---

*Remember: This library could validate a breakthrough in mathematics. Every line of code matters. No compromises on correctness.*