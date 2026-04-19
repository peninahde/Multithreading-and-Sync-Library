# This pulls the official Ubuntu 24.04 image
FROM ubuntu:24.04

# This updates the system and installs GCC 13 and other essential tools
RUN apt-get update && apt-get install -y \
    gcc-13 \
    g++-13 \
    gdb \
    make \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set GCC 13 as the default 'gcc' command
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 100 \
    --slave /usr/bin/g++ g++ /usr/bin/g++-13