#!/bin/bash

# Run the interactive demo for libadic
# This showcases the mathematical capabilities of the library

echo "Starting libadic interactive demonstration..."
echo

# Check if we need to build first
if [ ! -f build/interactive_demo ]; then
    echo "Building interactive demo..."
    mkdir -p build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make interactive_demo
    cd ..
fi

# Run the demo
if [ -f build/interactive_demo ]; then
    ./build/interactive_demo
else
    echo "Error: Could not build interactive demo"
    echo "Please ensure you have GMP and MPFR libraries installed:"
    echo "  Ubuntu/Debian: sudo apt-get install libgmp-dev libmpfr-dev"
    echo "  macOS: brew install gmp mpfr"
    exit 1
fi