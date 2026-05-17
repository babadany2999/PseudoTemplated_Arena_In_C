#!/bin/bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

BUILD_DIR="build"
mkdir -p "$BUILD_DIR"

echo "Building GoogleTest..."

GTEST_DIR="external/googletest"

GTEST_SRC_DIR="$GTEST_DIR/googletest/src"

mkdir -p "$BUILD_DIR/gtest"

# gtest core
g++ -I"$GTEST_DIR/googletest" \
    -I"$GTEST_DIR/googletest/include" \
    -c "$GTEST_DIR/googletest/src/gtest-all.cc" \
    -o "$BUILD_DIR/gtest/gtest.o"

# gtest main (provides main())
g++ -I"$GTEST_DIR/googletest" \
    -I"$GTEST_DIR/googletest/include" \
    -c "$GTEST_DIR/googletest/src/gtest_main.cc" \
    -o "$BUILD_DIR/gtest/gtest_main.o"

echo "Building your library..."

SRC_FILES=$(find source -name "*.c")

OBJ_FILES=""

for src in $SRC_FILES; do
    obj="$BUILD_DIR/$(basename "${src%.c}.o")"
    gcc  -Wall -Wextra -Werror -std=c23 -Iinclude -c "$src" -o "$obj"
    OBJ_FILES="$OBJ_FILES $obj"
done

echo "Building tests..."

TEST_FILES=$(find tests -name "*.cpp")

for test in $TEST_FILES; do
    name=$(basename "$test" .cpp)
    bin="$BUILD_DIR/$name"

    g++ -Wall -Wextra -Werror -Iinclude \
        -std=c++23 \
        -I"$GTEST_DIR/googletest/include" \
        -I"$ROOT_DIR/source" \
        -I"$ROOT_DIR/tests" \
        "$test" \
        $OBJ_FILES \
        "$BUILD_DIR/gtest/gtest.o" \
        "$BUILD_DIR/gtest/gtest_main.o" \
        -pthread \
        -o "$bin"

    echo "== Running $name =="
    "$bin"
done