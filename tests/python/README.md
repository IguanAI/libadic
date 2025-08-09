# Python Test Suite

This directory contains Python test scripts for the libadic library bindings.

## Test Files

### Core Functionality Tests
- **`test_python_simple.py`** - Basic Python binding tests
- **`test_python_bindings.py`** - Comprehensive binding validation
- **`test_api_summary.py`** - API completeness verification

### Mathematical Validation Tests
- **`test_reid_li_working.py`** - Reid-Li criterion basic tests
- **`test_reid_li_comprehensive.py`** - Comprehensive Reid-Li validation
- **`test_simple_working.py`** - Simple mathematical property tests
- **`verify_mathematical_correctness.py`** - Mathematical identity verification

## Running Tests

```bash
# Run all Python tests
cd tests/python
python -m pytest

# Run individual test files
python test_python_simple.py
python test_reid_li_working.py

# Comprehensive validation
python test_reid_li_comprehensive.py
python verify_mathematical_correctness.py
```

## Requirements

- Python 3.8+
- libadic Python bindings installed
- pytest (for running test suite)

## Test Coverage

These tests validate:
- Python binding completeness and correctness
- p-adic arithmetic operations
- Dirichlet character enumeration and evaluation
- L-function computations
- Reid-Li criterion mathematical identities
- Precision tracking and error handling