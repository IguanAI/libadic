#!/usr/bin/env python3
"""
Generate comprehensive documentation for libadic.
Creates multiple documentation formats for complete coverage.
"""

import os
import sys
import inspect
import textwrap
from typing import List, Dict, Any

sys.path.insert(0, '/mnt/c/Users/asmit/github/libadic/build')

try:
    import libadic
except ImportError:
    print("Error: libadic module not found. Please build the library first.")
    sys.exit(1)

def get_class_methods(cls) -> List[tuple]:
    """Extract all methods and properties from a class."""
    methods = []
    for name in dir(cls):
        if not name.startswith('_') or name in ['__init__', '__str__', '__repr__', 
                                                  '__add__', '__sub__', '__mul__', 
                                                  '__div__', '__pow__', '__eq__']:
            try:
                attr = getattr(cls, name)
                if callable(attr) or isinstance(attr, property):
                    doc = inspect.getdoc(attr) or "No documentation available"
                    methods.append((name, doc))
            except:
                pass
    return methods

def get_module_functions(module) -> List[tuple]:
    """Extract all module-level functions."""
    functions = []
    for name in dir(module):
        if not name.startswith('_'):
            attr = getattr(module, name)
            if callable(attr) and not inspect.isclass(attr):
                doc = inspect.getdoc(attr) or "No documentation available"
                functions.append((name, doc))
    return functions

def generate_api_reference():
    """Generate detailed API reference documentation."""
    
    output = []
    output.append("# libadic Python API Reference\n")
    output.append("*Comprehensive documentation for the libadic p-adic arithmetic library*\n")
    output.append("\n## Table of Contents\n")
    
    # Get all classes
    classes = []
    for name in dir(libadic):
        attr = getattr(libadic, name)
        if inspect.isclass(attr):
            classes.append((name, attr))
    
    # Generate TOC
    output.append("\n### Core Classes\n")
    for name, _ in sorted(classes):
        output.append(f"- [{name}](#{name.lower()})")
    
    output.append("\n### Module Functions\n")
    output.append("- [Character Functions](#character-functions)")
    output.append("- [L-Functions](#l-functions)")
    output.append("- [Special Functions](#special-functions)")
    output.append("- [Utility Functions](#utility-functions)")
    
    output.append("\n---\n")
    
    # Document each class
    output.append("\n## Core Classes\n")
    
    for class_name, cls in sorted(classes):
        output.append(f"\n### {class_name}\n")
        class_doc = inspect.getdoc(cls) or "No class documentation available"
        output.append(f"{class_doc}\n")
        
        # Get methods
        methods = get_class_methods(cls)
        
        if methods:
            output.append(f"\n#### Methods and Properties\n")
            for method_name, method_doc in sorted(methods):
                output.append(f"\n##### `{method_name}`\n")
                output.append(f"```python\n{method_doc}\n```\n")
    
    # Document module functions
    output.append("\n## Module Functions\n")
    
    functions = get_module_functions(libadic)
    
    # Categorize functions
    char_funcs = [f for f in functions if 'character' in f[0].lower() or 'enumerate' in f[0]]
    l_funcs = [f for f in functions if 'kubota' in f[0] or 'compute' in f[0] or 'l_' in f[0].lower()]
    special_funcs = [f for f in functions if 'gamma' in f[0] or 'log' in f[0] or 'bernoulli' in f[0]]
    util_funcs = [f for f in functions if f not in char_funcs + l_funcs + special_funcs]
    
    if char_funcs:
        output.append("\n### Character Functions\n")
        for func_name, func_doc in sorted(char_funcs):
            output.append(f"\n#### `{func_name}`\n")
            output.append(f"```python\n{func_doc}\n```\n")
    
    if l_funcs:
        output.append("\n### L-Functions\n")
        for func_name, func_doc in sorted(l_funcs):
            output.append(f"\n#### `{func_name}`\n")
            output.append(f"```python\n{func_doc}\n```\n")
    
    if special_funcs:
        output.append("\n### Special Functions\n")
        for func_name, func_doc in sorted(special_funcs):
            output.append(f"\n#### `{func_name}`\n")
            output.append(f"```python\n{func_doc}\n```\n")
    
    if util_funcs:
        output.append("\n### Utility Functions\n")
        for func_name, func_doc in sorted(util_funcs):
            output.append(f"\n#### `{func_name}`\n")
            output.append(f"```python\n{func_doc}\n```\n")
    
    return '\n'.join(output)

def generate_user_guide():
    """Generate user guide with examples and tutorials."""
    
    guide = """# libadic User Guide

## Introduction

libadic is a high-performance C++ library with Python bindings for p-adic arithmetic and the Reid-Li criterion for the Riemann Hypothesis. This guide covers installation, basic usage, and advanced mathematical computations.

## Installation

### Prerequisites
- C++17 compatible compiler
- GMP and MPFR libraries
- Python 3.8+ with pybind11
- CMake 3.10+

### Building from Source
```bash
# Install dependencies
sudo apt-get install libgmp-dev libmpfr-dev python3-dev

# Clone and build
git clone https://github.com/yourusername/libadic.git
cd libadic
mkdir build && cd build
cmake ..
make -j$(nproc)

# The Python module is at build/libadic.so
```

### Python Setup
```python
import sys
sys.path.append('/path/to/libadic/build')
import libadic
```

## Quick Start

### Basic p-adic Arithmetic

```python
import libadic

# Create p-adic integers
x = libadic.Zp(7, 20, 15)  # 15 in Z_7 with precision O(7^20)
y = libadic.Zp(7, 20, 8)   # 8 in Z_7

# Arithmetic operations
z = x + y  # Addition
w = x * y  # Multiplication
q = x / y  # Division (y must be a unit)

# p-adic numbers (field elements)
a = libadic.Qp(7, 20, 14)  # 14 = 2*7, so valuation 1
b = libadic.Qp.from_rational(22, 7, 5, 20)  # 22/7 in Q_5
```

### Working with Dirichlet Characters

```python
# Enumerate primitive characters
p = 7
chars = libadic.enumerate_primitive_characters(p, p)
print(f"Found {len(chars)} primitive characters mod {p}")

# Work with a character
chi = chars[0]
print(f"Conductor: {chi.get_conductor()}")
print(f"Is odd: {chi.is_odd()}")

# Evaluate character
val_int = chi.evaluate_at(3)  # Returns integer
val_zp = chi.evaluate(3, 20)   # Returns Zp with precision 20

# Character arithmetic
chi_product = chars[0] * chars[1]  # Multiply characters
chi_power = chars[0] ** 2          # Square a character
```

### Computing L-Functions

```python
# Kubota-Leopoldt p-adic L-function
chi = chars[0]
L_val = libadic.kubota_leopoldt(0, chi, 20)
print(f"L_p(0, χ) = {L_val}")

# L-function derivative (for odd characters)
if chi.is_odd():
    L_deriv = libadic.kubota_leopoldt_derivative(0, chi, 20)
    print(f"L'_p(0, χ) = {L_deriv}")

# Compute Bernoulli numbers
B1 = libadic.compute_B1_chi(chi, 20)
print(f"B_{{1,χ}} = {B1}")
```

### p-adic Special Functions

```python
# Morita's p-adic Gamma function
gamma_val = libadic.gamma_p(5, 7, 20)  # Γ_7(5)
print(f"Γ_7(5) = {gamma_val}")

# With Zp argument
x = libadic.Zp(7, 20, 5)
gamma_x = libadic.gamma_p(x)

# p-adic logarithm
log_val = libadic.log_p(libadic.Qp(7, 20, 8))
log_gamma = libadic.log_gamma_p(gamma_val)
```

## Advanced Topics

### Reid-Li Criterion Validation

The Reid-Li criterion relates character sums to L-function values:
- For odd χ: Φ_p^(odd)(χ) = L'_p(0, χ)
- For even χ: Φ_p^(even)(χ) = L_p(0, χ)

```python
def validate_reid_li(chi, p, precision):
    \"\"\"Validate Reid-Li criterion for a character.\"\"\"
    if chi.is_odd():
        # Compute Φ_p^(odd)(χ) = Σ_{a=1}^{p-1} χ(a) * log_p(Γ_p(a))
        phi = libadic.Qp(p, precision, 0)
        for a in range(1, p):
            chi_a = chi.evaluate(a, precision)
            if not chi_a.is_zero():
                gamma_a = libadic.gamma_p(a, p, precision)
                # Would need log_gamma here
                # Full implementation requires careful convergence handling
        
        # Compare with L'_p(0, χ)
        psi = libadic.kubota_leopoldt_derivative(0, chi, precision)
        return phi, psi
    else:
        # Even character case
        L_val = libadic.kubota_leopoldt(0, chi, precision)
        return None, L_val
```

### Precision Management

```python
# Precision is tracked through operations
x = libadic.Zp(7, 20, 15)  # precision 20
y = libadic.Zp(7, 15, 8)   # precision 15
z = x + y  # precision = min(20, 15) = 15

# Check precision
print(f"Precision of z: {z.precision}")

# Reduce precision explicitly
x_reduced = x.with_precision(10)
```

### Error Handling

```python
try:
    # Division by non-unit
    x = libadic.Zp(7, 20, 14)  # 14 = 2*7, not a unit
    y = libadic.Zp(7, 20, 3)
    z = y / x  # Will raise error
except Exception as e:
    print(f"Error: {e}")

# Check convergence for logarithm
x = libadic.Qp(7, 20, 8)
# log_p requires x ≡ 1 (mod p) for convergence
```

## Mathematical Background

### p-adic Numbers
The p-adic numbers Q_p are the completion of Q with respect to the p-adic norm |x|_p = p^(-v_p(x)).

### Dirichlet Characters
A Dirichlet character χ mod n is a completely multiplicative function from (Z/nZ)* to C*.

### Reid-Li Criterion
The Reid-Li criterion provides a p-adic approach to the Riemann Hypothesis through special values of p-adic L-functions.

## API Patterns

### Factory Methods
```python
# Preferred way to create from rationals
q = libadic.Qp.from_rational(22, 7, 5, 20)

# Alternative construction
q = libadic.Qp(5, 20, 22) / libadic.Qp(5, 20, 7)
```

### Property Access
```python
# Most attributes are properties
chi.get_modulus()    # Method call
chi.character_values  # Direct access (now public)
q.valuation          # Property
```

## Performance Tips

1. **Reuse precision**: Operations maintain minimum precision
2. **Cache characters**: Character enumeration is expensive
3. **Batch operations**: Use vectorized operations when possible
4. **Clear caches**: Use `libadic.clear_l_cache()` to free memory

## Troubleshooting

### Common Issues

**ImportError**: Module not found
- Ensure build directory is in Python path
- Check that build completed successfully

**Convergence errors**: p-adic logarithm
- Ensure input satisfies convergence conditions
- For log_p: need x ≡ 1 (mod p)

**Precision loss**: Unexpected precision reduction
- Check for division operations
- Verify input precisions

## Further Reading

- Washington, L.C. "Introduction to Cyclotomic Fields"
- Koblitz, N. "p-adic Numbers, p-adic Analysis, and Zeta-Functions"
- Reid, L. & Li, X. "p-adic L-functions and the Riemann Hypothesis"
"""
    return guide

def generate_mathematical_reference():
    """Generate mathematical reference documentation."""
    
    math_ref = """# Mathematical Reference for libadic

## Core Mathematical Objects

### p-adic Integers (Z_p)

The ring of p-adic integers Z_p consists of formal series:
```
x = a_0 + a_1·p + a_2·p² + ... where 0 ≤ a_i < p
```

**Properties:**
- Z_p is a complete discrete valuation ring
- Units: Z_p* = {x ∈ Z_p : x ≢ 0 (mod p)}
- Every x ∈ Z_p* has a multiplicative inverse

### p-adic Numbers (Q_p)

The field Q_p of p-adic numbers:
```
x = p^v · u where v ∈ Z (valuation) and u ∈ Z_p* (unit)
```

**p-adic Norm:**
```
|x|_p = p^(-v_p(x))
```

### Dirichlet Characters

A Dirichlet character χ modulo n is a homomorphism:
```
χ: (Z/nZ)* → C*
```

**Properties:**
- Completely multiplicative: χ(ab) = χ(a)χ(b)
- χ(a) = 0 if gcd(a,n) > 1
- Primitive if conductor equals modulus

**Character Group:**
- Characters form group under multiplication
- |{χ mod n}| = φ(n) (Euler's totient)
- Dual group of (Z/nZ)*

### p-adic L-Functions

#### Kubota-Leopoldt L-function

For a Dirichlet character χ, the p-adic L-function L_p(s, χ) satisfies:

**Special Values:**
```
L_p(1-n, χ) = -(1 - χ(p)p^(n-1)) · B_{n,χ}/n
```

where B_{n,χ} are generalized Bernoulli numbers.

**At s=0:**
```
L_p(0, χ) = -(1 - χ(p)p^(-1)) · B_{1,χ}
```

#### Generalized Bernoulli Numbers

Definition:
```
B_{n,χ} = n^(-1) · Σ_{a=1}^{f} χ(a) · B_n(a/f)
```
where f is the conductor of χ and B_n(x) are Bernoulli polynomials.

**Key Values:**
- B_0 = 1
- B_1 = -1/2 (or +1/2 depending on convention)
- B_{2k+1} = 0 for k ≥ 1 (except B_1)

### Morita's p-adic Gamma Function

The p-adic Gamma function Γ_p is defined by:

**Product Formula:**
```
Γ_p(x) = lim_{n→∞} (-1)^n · ∏_{0<k<p^n, (k,p)=1} k
```

**Functional Equation:**
```
Γ_p(x+1) = -x · Γ_p(x) for x ∉ Z_p*
```

**Special Values:**
- Γ_p(1) ≠ 0
- Γ_p(p) = 0
- Γ_p(x) · Γ_p(1-x) = ±1 (reflection formula)

### p-adic Logarithm

The p-adic logarithm is defined by the series:
```
log_p(x) = Σ_{n=1}^∞ (-1)^(n+1) · (x-1)^n/n
```

**Convergence:**
- For p ≠ 2: requires |x-1|_p < 1, i.e., x ≡ 1 (mod p)
- For p = 2: requires x ≡ 1 (mod 4)

**Properties:**
- log_p(xy) = log_p(x) + log_p(y) (when both converge)
- log_p(x^n) = n · log_p(x)

## Reid-Li Criterion

### Statement

For primitive Dirichlet character χ mod p:

**Odd Characters (χ(-1) = -1):**
```
Φ_p^(odd)(χ) = Σ_{a=1}^{p-1} χ(a) · log_p(Γ_p(a)) = L'_p(0, χ)
```

**Even Characters (χ(-1) = 1):**
```
Φ_p^(even)(χ) = Σ_{a=1}^{p-1} χ(a) · log_p(a/(p-1)) = L_p(0, χ)
```

### Significance

The Reid-Li criterion provides:
1. A p-adic approach to the Riemann Hypothesis
2. Explicit formulas relating character sums to L-values
3. Computational verification methods for RH

## Algorithms

### Hensel Lifting

For solving f(x) ≡ 0 (mod p^n):

```python
def hensel_lift(f, df, x0, p, n):
    \"\"\"
    Lift solution from mod p^k to mod p^(k+1)
    Requires: f(x0) ≡ 0 (mod p^k), df(x0) ≢ 0 (mod p)
    \"\"\"
    x = x0
    pk = p^k
    for k in range(current_precision, n):
        # Newton step
        fx = f(x) % p^(k+1)
        dfx = df(x) % p^k
        x = x - (fx / dfx) * p^k
        pk *= p
    return x % p^n
```

### Character Enumeration

Characters mod n are determined by their values on generators of (Z/nZ)*:

```python
def enumerate_characters(n, p):
    \"\"\"
    Generate all Dirichlet characters mod n.
    \"\"\"
    generators = find_generators(n)
    generator_orders = [order_in_group(g, n) for g in generators]
    
    characters = []
    # Iterate over all possible values on generators
    for values in product(*[range(ord) for ord in generator_orders]):
        chi = DirichletCharacter(n, p, values)
        characters.append(chi)
    
    return characters
```

### Teichmüller Lift

The Teichmüller character ω: (Z/pZ)* → Z_p*:

```python
def teichmuller(a, p, precision):
    \"\"\"
    Compute Teichmüller lift ω(a).
    ω(a) is the unique (p-1)-th root of unity ≡ a (mod p)
    \"\"\"
    # Start with a mod p
    x = a % p
    
    # Iterate: x_{n+1} = x_n^p
    for _ in range(precision):
        x = pow(x, p, p^precision)
    
    return x
```

## Precision and Convergence

### Precision Rules

1. **Addition/Subtraction**: precision = min(prec1, prec2)
2. **Multiplication**: precision = min(prec1, prec2)
3. **Division**: precision may decrease if divisor has p-factor
4. **Series**: precision determines truncation point

### Convergence Criteria

**p-adic Exponential:**
- Requires |x|_p < p^(-1/(p-1))

**p-adic Logarithm:**
- Requires |x-1|_p < 1

**Newton-Hensel:**
- Requires non-zero derivative mod p
- Doubles correct digits each iteration

## Implementation Notes

### Memory Management
- Cache frequently used values (Bernoulli numbers, L-values)
- Clear caches periodically with `clear_cache()` functions

### Numerical Stability
- Track precision explicitly through all operations
- Use exact arithmetic (no floating point)
- Verify convergence conditions before series evaluation

### Performance Optimization
- Precompute character values on small residues
- Use binary exponentiation for powers
- Cache Teichmüller lifts

## References

1. **Koblitz, N.** (1984). *p-adic Numbers, p-adic Analysis, and Zeta-Functions*. Springer.

2. **Washington, L.C.** (1997). *Introduction to Cyclotomic Fields*. Springer.

3. **Iwasawa, K.** (1972). *Lectures on p-adic L-functions*. Princeton University Press.

4. **Reid, L. & Li, X.** (2023). *A p-adic Approach to the Riemann Hypothesis via the Reid-Li Criterion*.

5. **Morita, Y.** (1975). *A p-adic analogue of the Γ-function*. J. Fac. Sci. Univ. Tokyo.

6. **Kubota, T. & Leopoldt, H.W.** (1964). *Eine p-adische Theorie der Zetawerte*.
"""
    return math_ref

def generate_all_documentation():
    """Generate all documentation files."""
    
    print("Generating comprehensive documentation for libadic...")
    
    # Create docs directory
    os.makedirs("docs", exist_ok=True)
    
    # Generate API reference
    print("  1. Generating API reference...")
    api_ref = generate_api_reference()
    with open("docs/API_REFERENCE.md", "w") as f:
        f.write(api_ref)
    print("     ✓ API_REFERENCE.md created")
    
    # Generate user guide
    print("  2. Generating user guide...")
    user_guide = generate_user_guide()
    with open("docs/USER_GUIDE.md", "w") as f:
        f.write(user_guide)
    print("     ✓ USER_GUIDE.md created")
    
    # Generate mathematical reference
    print("  3. Generating mathematical reference...")
    math_ref = generate_mathematical_reference()
    with open("docs/MATHEMATICAL_REFERENCE.md", "w") as f:
        f.write(math_ref)
    print("     ✓ MATHEMATICAL_REFERENCE.md created")
    
    # Generate README
    print("  4. Generating main README...")
    readme = """# libadic - High-Performance p-adic Arithmetic Library

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/std/the-standard)
[![Python 3.8+](https://img.shields.io/badge/Python-3.8%2B-blue.svg)](https://www.python.org/)

A comprehensive C++ library with Python bindings for p-adic arithmetic, Dirichlet characters, and validation of the Reid-Li criterion for the Riemann Hypothesis.

## Features

- **Complete p-adic arithmetic** - Zp (integers) and Qp (field) with explicit precision tracking
- **Dirichlet characters** - Enumeration, evaluation, and arithmetic operations
- **p-adic L-functions** - Kubota-Leopoldt L-functions and derivatives
- **Special functions** - Morita's p-adic Gamma, p-adic logarithm, Bernoulli numbers
- **Reid-Li validation** - Tools for verifying the Reid-Li criterion
- **High performance** - Built on GMP/MPFR with optimized algorithms
- **Python bindings** - Complete Python API via pybind11

## Quick Start

```python
import libadic

# p-adic arithmetic
x = libadic.Zp(7, 20, 15)  # 15 in Z_7 with precision O(7^20)
y = libadic.Qp.from_rational(22, 7, 5, 20)  # 22/7 in Q_5

# Dirichlet characters
chars = libadic.enumerate_primitive_characters(7, 7)
chi = chars[0]
L_val = libadic.kubota_leopoldt(0, chi, 20)  # L_7(0, χ)

# p-adic Gamma function
gamma = libadic.gamma_p(5, 7, 20)  # Γ_7(5)
```

## Documentation

- [User Guide](docs/USER_GUIDE.md) - Installation, tutorials, and examples
- [API Reference](docs/API_REFERENCE.md) - Complete API documentation
- [Mathematical Reference](docs/MATHEMATICAL_REFERENCE.md) - Mathematical background and algorithms

## Building from Source

```bash
# Install dependencies
sudo apt-get install libgmp-dev libmpfr-dev python3-dev

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run tests
ctest --verbose
```

## Mathematical Background

This library implements the Reid-Li criterion, which provides a p-adic approach to the Riemann Hypothesis through the identity:

- For odd characters: Φ_p^(odd)(χ) = L'_p(0, χ)
- For even characters: Φ_p^(even)(χ) = L_p(0, χ)

## Contributing

Contributions are welcome! Please ensure:
- Code compiles with `-Wall -Wextra -Wpedantic`
- All tests pass
- Mathematical correctness is maintained

## License

MIT License - see LICENSE file for details.

## Authors

- [Your Name]

## Acknowledgments

- GMP and MPFR developers
- pybind11 community
- Reid & Li for the mathematical framework
"""
    with open("README.md", "w") as f:
        f.write(readme)
    print("     ✓ README.md created")
    
    # Generate example notebook content
    print("  5. Generating example notebook...")
    notebook_content = '''
{
 "cells": [
  {
   "cell_type": "markdown",
   "metadata": {},
   "source": [
    "# libadic Tutorial - Reid-Li Criterion Validation\\n",
    "This notebook demonstrates using libadic for p-adic computations and Reid-Li validation."
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {},
   "outputs": [],
   "source": [
    "import sys\\n",
    "sys.path.append('/path/to/libadic/build')\\n",
    "import libadic\\n",
    "print(f\\"libadic version: {libadic.__version__}\\")"
   ]
  },
  {
   "cell_type": "markdown",
   "metadata": {},
   "source": [
    "## Basic p-adic Arithmetic"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "metadata": {},
   "outputs": [],
   "source": [
    "# Create p-adic integers\\n",
    "p = 7\\n",
    "precision = 20\\n",
    "x = libadic.Zp(p, precision, 15)\\n",
    "y = libadic.Zp(p, precision, 8)\\n",
    "z = x + y\\n",
    "print(f\\"{x} + {y} = {z}\\")"
   ]
  }
 ],
 "metadata": {
  "kernelspec": {
   "display_name": "Python 3",
   "language": "python",
   "name": "python3"
  }
 },
 "nbformat": 4,
 "nbformat_minor": 4
}
'''
    with open("docs/tutorial.ipynb", "w") as f:
        f.write(notebook_content)
    print("     ✓ tutorial.ipynb created")
    
    print("\n✅ Documentation generation complete!")
    print("\nGenerated files:")
    print("  📁 docs/")
    print("  ├── 📄 API_REFERENCE.md - Complete API documentation")
    print("  ├── 📄 USER_GUIDE.md - Installation and usage guide")
    print("  ├── 📄 MATHEMATICAL_REFERENCE.md - Mathematical background")
    print("  └── 📓 tutorial.ipynb - Jupyter notebook tutorial")
    print("  📄 README.md - Main project README")
    
    return True

if __name__ == "__main__":
    generate_all_documentation()