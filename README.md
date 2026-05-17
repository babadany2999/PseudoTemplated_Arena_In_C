# C23 X-Macro Dynamic Allocator (Practice Project)

[![Test CI](https://github.com/your-username/your-repo/actions/workflows/test.yaml/badge.svg)](https://github.com/babadany2999/PseudoTemplated_Arena_In_C/test)

> ⚠️ **Disclaimer:** This is a personal learning project and sandbox. It is **not** intended for production use, and I don't recommend using it in real-world applications. It exists solely as an exercise to explore advanced C programming concepts and to serve as a portfolio piece.

## 🧠 Motivation & Learning Goals

The goal of this project was to implement a generic, type-safe dynamic array (vector) in C without relying on `void*` pointers or runtime type erasure. To achieve this, I used the **X-Macro** pattern. 

By building this, I practiced and demonstrated the following concepts:
* **Advanced C Preprocessor:** Using X-Macros to generate strongly-typed structs and function implementations at compile time.
* **Modern C/C++ Standards:** Writing the core logic in **C23** and testing it with **C++23**.
* **Memory Management:** Implementing a custom allocator with a memory-doubling reallocation strategy.
* **Cross-Language Testing:** Integrating a pure C codebase with C++'s **GoogleTest** framework, utilizing template fixtures to stress-test the generated C macros.
* **Build Systems:** Writing a raw bash build script (`test.sh`) to manually compile objects and link them with GoogleTest, rather than relying on CMake, to deeply understand the compiler toolchain.

## ⚙️ Technical Overview

### The X-Macro Architecture
In typical C, generic collections usually involve `void*` casting, which sacrifices type safety. This project solves that by defining a single list of types in a `.x` file:

```c
// dynamic_allocator.x
DYNAMIC_ALLOCATOR_DEF(Int, int)
DYNAMIC_ALLOCATOR_DEF(Vector3, struct Vector3)