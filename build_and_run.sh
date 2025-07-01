#!/bin/bash
set -e

BUILD_DIR="build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake ..
make

if [[ "$OSTYPE" == "darwin"* ]]; then
    if [[ -f ../bin/macOS/main ]]; then
        ../bin/macOS/main
    else
        echo "ERROR: Executable not found at ../bin/macOS/main"
        exit 1
    fi
else
    if [[ -f ../bin/Linux/main ]]; then
        ../bin/Linux/main
    else
        echo "ERROR: Executable not found at ../bin/Linux/main"
        exit 1
    fi
fi

cd ..