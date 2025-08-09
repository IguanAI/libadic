# Base image with development tools
FROM ubuntu:22.04

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install essential build tools and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libgmp-dev \
    libmpfr-dev \
    valgrind \
    gdb \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /libadic

# Copy project files
COPY . .

# Create build directory
RUN mkdir -p build

# Default command - can be overridden
CMD ["/bin/bash"]