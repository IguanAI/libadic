#!/bin/bash
# Build Debian packages for libadic

set -e

echo "Building Debian packages for libadic..."

# Check dependencies
if ! command -v dpkg-buildpackage &> /dev/null; then
    echo "Error: dpkg-buildpackage not found. Install build-essential and devscripts:"
    echo "  sudo apt-get install build-essential devscripts debhelper"
    exit 1
fi

# Clean any previous builds
rm -rf debian/tmp debian/*.log debian/*.substvars debian/*.debhelper debian/files
rm -f ../libadic*.deb ../libadic*.dsc ../libadic*.tar.* ../libadic*.changes

# Build the package
echo "Building packages..."
dpkg-buildpackage -us -uc -b

echo ""
echo "Packages built successfully!"
echo "Generated files:"
ls -la ../libadic*.deb 2>/dev/null || echo "No .deb files found"

echo ""
echo "To install locally:"
echo "  sudo dpkg -i ../libadic*.deb"
echo "  sudo apt-get install -f  # Fix any dependency issues"

echo ""
echo "To test installation:"
echo "  # After installation, you can use:"
echo "  #include <libadic/zp.h>"
echo "  # Link with: -ladic -lgmp -lmpfr"