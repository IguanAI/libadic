# Quick Start Guide for libadic

## Prerequisites

Before building libadic, ensure you have the following dependencies installed:

### Required Dependencies
- **CMake** (>= 3.15): Build system generator
- **C++ Compiler** with C++17 support (GCC 7+, Clang 5+, or MSVC 2017+)
- **GMP** (GNU Multiple Precision Arithmetic Library)
- **Python 3.7+** (for Python bindings)

### Installing Dependencies

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y cmake build-essential libgmp-dev python3-dev
```

#### macOS (with Homebrew)
```bash
brew install cmake gmp python
```

#### Windows (with vcpkg)
```powershell
vcpkg install gmp
```

## Building the Library

### Quick Build (Recommended)
```bash
# Clone the repository
git clone https://github.com/vadyushkins/libadic.git
cd libadic

# Create build directory
mkdir build && cd build

# Configure and build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Run tests to verify installation
ctest --verbose
```

### Install Python Bindings
```bash
# From the repository root
pip install .

# Verify installation
python -c "import libadic; print(f'libadic version: {libadic.__version__}')"
```

## Running Validation Tests

### Quick Validation
```bash
# Run C++ mathematical validation tests
./build/tests/test_math_validations

# Run Python Reid-Li tests
python tests/python/test_reid_li_working.py
```

### Full Validation Suite
```bash
# Run complete validation (includes comparison with other libraries)
cd docs/validation
./run_validation.sh
```

## Verifying Reid-Li Implementation

The library's core feature is the Reid-Li criterion implementation. To verify it works:

```python
import libadic

# Set up parameters
p = 5
precision = 15

# Get primitive characters
chars = libadic.enumerate_primitive_characters(p, p)

# Find an odd character and compute L'_p(0, χ)
for chi in chars:
    if chi.is_odd():
        # This internally computes Φ_p(χ) = Σ χ(a) log Γ_p(a)
        lp_derivative = libadic.kubota_leopoldt_derivative(0, chi, precision)
        print(f"L'_p(0, χ) = {lp_derivative}")
        break
```

## Troubleshooting

### CMake not found
If you get "cmake: command not found", install CMake using your package manager as shown above.

### GMP not found
If CMake reports "Could NOT find GMP", ensure GMP is installed and set the path:
```bash
cmake .. -DGMP_ROOT=/path/to/gmp
```

### Python module import error
If Python can't import libadic after installation:
```bash
# Ensure you're using the same Python version for building and running
python --version
pip show libadic

# Reinstall with verbose output
pip install . -v
```

## Next Steps

- Read [MATHEMATICAL_REFERENCE.md](docs/MATHEMATICAL_REFERENCE.md) for theoretical background
- See [examples/](examples/) directory for usage examples
- Check [tests/](tests/) for comprehensive test cases
- Review [VALIDATION_REPORT.md](docs/validation/validation_output/VALIDATION_REPORT.md) for proof of uniqueness