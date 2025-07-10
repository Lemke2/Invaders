#!/bin/bash

# Compiler and paths
CC=clang
RAYLIB_INCLUDE="/usr/local/include/raylib"  # Path to raylib.h
RAYLIB_LIB="/usr/local/include/raylib/src"  # Path to libraylib.a
OUTPUT="invadersgame"

# Compile command for static linking
$CC src/*.cpp -I$RAYLIB_INCLUDE -L$RAYLIB_LIB -l:libraylib.a \
    -lGL -lm -lpthread -ldl -lX11 -o $OUTPUT

if [ $? -eq 0 ]; then
    echo "Running $OUTPUT..."
    ./$OUTPUT
fi