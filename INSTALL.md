# Installation Guide for libadic

## 🚀 Quick Install (Ubuntu/Debian)

### Option 1: Install from PPA (Recommended)
```bash
# Add the libadic PPA
sudo add-apt-repository ppa:your-username/libadic-stable
sudo apt-get update

# Install the library and development files
sudo apt-get install libadic-dev

# Optional: Install documentation
sudo apt-get install libadic-doc
```

### Option 2: Install from .deb Package
```bash
# Download the latest release from GitHub
wget https://github.com/yourusername/libadic/releases/download/v1.0.0/libadic_1.0.0-1_amd64.deb
wget https://github.com/yourusername/libadic/releases/download/v1.0.0/libadic-dev_1.0.0-1_amd64.deb

# Install packages
sudo dpkg -i libadic*.deb
sudo apt-get install -f  # Fix any dependency issues
```

## 📦 Building Packages from Source

### Prerequisites
```bash
sudo apt-get install build-essential devscripts debhelper cmake
sudo apt-get install libgmp-dev libmpfr-dev
```

### Build Debian Packages
```bash
git clone https://github.com/yourusername/libadic.git
cd libadic
./scripts/build_deb.sh
```

This creates:
- `libadic1_1.0.0-1_amd64.deb` - Runtime library
- `libadic-dev_1.0.0-1_amd64.deb` - Development files
- `libadic-doc_1.0.0-1_all.deb` - Documentation

### Install Locally
```bash
sudo dpkg -i ../libadic*.deb
```

## 🌐 Setting Up Your Own PPA

### 1. Prerequisites
- Create a [Launchpad](https://launchpad.net) account
- [Generate and upload GPG key](https://help.launchpad.net/YourAccount/ImportingYourOpenPGPKey)
- [Add SSH key](https://help.launchpad.net/YourAccount/CreatingAnSSHKeyPair)

### 2. Create PPA on Launchpad
1. Go to your Launchpad profile
2. Click "Create a new PPA"
3. Name it `libadic-stable`

### 3. Upload Package
```bash
cd libadic
./scripts/setup_ppa.sh
# Follow the prompts
```

## 🔧 Building from Source (Traditional)

### Standard CMake Build
```bash
git clone https://github.com/yourusername/libadic.git
cd libadic
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
```

### With Shared Libraries
```bash
cmake .. -DBUILD_SHARED_LIBS=ON
make -j$(nproc)
sudo make install
sudo ldconfig  # Update library cache
```

## 📝 Using libadic After Installation

### Include in Your Project
```cpp
#include <libadic/zp.h>
#include <libadic/padic_gamma.h>

using namespace libadic;

int main() {
    // Compute Morita's Gamma function
    Zp gamma = gamma_p(5, 7, 20);
    std::cout << gamma.to_string() << std::endl;
    return 0;
}
```

### Compile Your Code
```bash
# With pkg-config (if available)
g++ -std=c++17 mycode.cpp `pkg-config --cflags --libs libadic`

# Manual linking
g++ -std=c++17 mycode.cpp -ladic -lgmp -lmpfr
```

### CMake Integration
```cmake
find_package(libadic REQUIRED)
target_link_libraries(myapp libadic::adic)
```

## 🐳 Docker Installation

```bash
docker pull yourusername/libadic:latest
docker run -it yourusername/libadic:latest
```

## 📋 Package Contents

### libadic1 (Runtime)
- `/usr/lib/x86_64-linux-gnu/libadic.so.1` - Shared library

### libadic-dev (Development)
- `/usr/include/libadic/*.h` - Header files
- `/usr/lib/x86_64-linux-gnu/libadic.so` - Symlink for linking
- `/usr/lib/x86_64-linux-gnu/libadic.a` - Static library
- `/usr/lib/x86_64-linux-gnu/cmake/libadic/` - CMake config

### libadic-doc (Documentation)
- `/usr/share/doc/libadic/` - Documentation
- `/usr/share/doc/libadic/examples/` - Example code
- `/usr/share/doc/libadic/validation/` - Uniqueness proof

## 🔍 Verifying Installation

```bash
# Check if installed
dpkg -l | grep libadic

# Find installed files
dpkg -L libadic-dev

# Test compilation
echo '#include <libadic/zp.h>
int main() { libadic::Zp x(5, 10, 1); return 0; }' > test.cpp
g++ -std=c++17 test.cpp -ladic -lgmp -lmpfr && ./a.out && echo "Success!"
```

## 🆘 Troubleshooting

### Missing Dependencies
```bash
sudo apt-get install libgmp-dev libmpfr-dev
```

### Library Not Found
```bash
sudo ldconfig
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

### CMake Can't Find Package
```bash
export CMAKE_PREFIX_PATH=/usr/local:$CMAKE_PREFIX_PATH
```

## 📦 Creating Packages for Other Distributions

### RPM (Fedora/RHEL)
```bash
cd build
cpack -G RPM
```

### Arch Linux (AUR)
See `packaging/PKGBUILD` (coming soon)

### macOS (Homebrew)
```bash
brew tap yourusername/libadic
brew install libadic
```

## 📄 License

libadic is distributed under the MIT License. The Debian packages maintain this license.

## 🤝 Support

- [GitHub Issues](https://github.com/yourusername/libadic/issues)
- [Documentation](https://github.com/yourusername/libadic/tree/main/docs)

---

*libadic - The ONLY implementation of the Reid-Li criterion for the Riemann Hypothesis*