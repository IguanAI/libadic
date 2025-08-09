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
