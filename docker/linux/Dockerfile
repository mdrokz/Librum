# Use Ubuntu 22.04 as the base image
FROM ubuntu:22.04 as builder

# Set non-interactive mode for apt-get
ENV DEBIAN_FRONTEND=noninteractive

# Set ARGs for QT versions
ARG QT_VER
ARG QT_SEMVER

# Install required packages
RUN apt-get update && apt-get install -y \
    build-essential \
    libgl1-mesa-dev \
    clang \
    pkg-config \
    libclang-dev \
    llvm-dev \
    cmake \
    g++ \
    make \
    git \
    wget \
    python3 \
    python3-pip \
    python3-venv

# Set working directory
WORKDIR /app

# Copy current directory contents to the container
COPY . .

# Download and extract QT sources
RUN wget https://download.qt.io/official_releases/qt/${QT_VER}/${QT_SEMVER}/single/qt-everywhere-src-${QT_SEMVER}.tar.xz -O qt-${QT_SEMVER}.tar.xz && \
    tar -xvf qt-${QT_SEMVER}.tar.xz && \
    rm qt-${QT_SEMVER}.tar.xz

# Build and install QT
RUN cd qt-everywhere-src-${QT_SEMVER} && \
    git submodule update --init && \
    mkdir build && \
    cd build && \
    ../configure -prefix /home/qt && \
    make -j$(nproc) && \
    make install

# Clean up QT sources and build the application
RUN rm -rf qt-everywhere-src-${QT_SEMVER} && \
    mkdir build-Release && \
    cd build-Release && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=Off -DCMAKE_PREFIX_PATH=/home/qt .. && \
    cmake --build . -j $(nproc)
