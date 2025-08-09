# libadic - High-Performance p-adic Arithmetic Library

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

### Core Documentation
- [**User Guide**](docs/USER_GUIDE.md) - Complete tutorials with step-by-step examples
- [**API Reference**](docs/API_REFERENCE.md) - Detailed API with working code examples  
- [**Mathematical Reference**](docs/MATHEMATICAL_REFERENCE.md) - Proofs, algorithms, and numerical examples

### Example Scripts
- [**Reid-Li Complete Validation**](examples/reid_li_complete.py) - Full Reid-Li criterion implementation
- [**Character Exploration**](examples/character_exploration.py) - Dirichlet character analysis
- [**Precision Management**](examples/precision_management.py) - Precision tracking and optimization

## Installation

### Python Package (Recommended)

```bash
# Install from PyPI (when available)
pip install libadic

# Or build from source
git clone https://github.com/IguanAI/libadic.git
cd libadic
pip install .
```

### Building from Source (C++ Library + Python Bindings)

```bash
# Install dependencies
sudo apt-get install libgmp-dev libmpfr-dev python3-dev

# Build C++ library and Python bindings
mkdir build && cd build
cmake -DBUILD_PYTHON_BINDINGS=ON ..
make -j$(nproc)

# Install Python module
cd python
pip install .

# Run tests
cd ../build && ctest --verbose
```

### C++ Only Build

```bash
# For C++ development only
mkdir build && cd build
cmake -DBUILD_PYTHON_BINDINGS=OFF ..
make -j$(nproc)
ctest --verbose
```

## Python API Usage

### Basic p-adic Arithmetic

```python
import libadic

# Create p-adic integers
p = 7
precision = 20
x = libadic.Zp(p, precision, 42)
y = libadic.Zp(p, precision, 13)

# Arithmetic operations
z = x + y * libadic.Zp(p, precision, 2)
print(f"Result: {z}")
print(f"p-adic digits: {z.digits()}")

# p-adic numbers (field)
a = libadic.Qp.from_rational(22, 7, p, precision)
b = libadic.Qp(p, precision, 5)
quotient = a / b
print(f"22/7 ÷ 5 in Q_7 = {quotient}")
```

### Dirichlet Characters and L-functions

```python
# Enumerate all primitive characters mod p
p = 11
chars = libadic.enumerate_primitive_characters(p, p)
print(f"Found {len(chars)} primitive characters mod {p}")

# Explore character properties
chi = chars[0]
print(f"Character order: {chi.get_order()}")
print(f"Is odd: {chi.is_odd()}")

# Compute L-function values
L_value = libadic.kubota_leopoldt(0, chi, precision)
print(f"L_p(0, χ) = {L_value}")

# For odd characters, compute derivative
if chi.is_odd():
    L_deriv = libadic.kubota_leopoldt_derivative(0, chi, precision)
    print(f"L'_p(0, χ) = {L_deriv}")
```

### Special Functions

```python
# p-adic Gamma function
gamma_5 = libadic.gamma_p(5, p, precision)
print(f"Γ_7(5) = {gamma_5}")

# p-adic logarithm (requires convergence condition)
x = libadic.Qp(p, precision, 1 + p)  # x ≡ 1 (mod p)
log_x = libadic.log_p(x)
print(f"log_7(8) = {log_x}")

# Square roots via Hensel lifting
a = libadic.Zp(p, precision, 4)
sqrt_a = a.sqrt()
print(f"√4 in Z_7 = {sqrt_a}")
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

- IguanAI Team
- Contributors on [GitHub](https://github.com/IguanAI/libadic/graphs/contributors)

## Acknowledgments

- GMP and MPFR developers
- pybind11 community
- Reid & Li for the mathematical framework
