#!/bin/bash

# Create build directories
mkdir -p obj bin

# Build the project
make

# Notify user
echo "Build completed. The binary is located in the bin directory."