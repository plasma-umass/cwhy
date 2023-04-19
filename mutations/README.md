# Mutations

> **Warning**
> Experimental.

The goal of this sub-project is to automatically generate small mutations in C++ source files, and then see if CWhy is
capable of providing a fix returning to the original source code.

## Build

```
[~] sudo apt install -y llvm-15 clang-15 libclang-15-dev
[cwhy/mutations] cmake .                         \
    -B build                                     \
    -G Ninja                                     \
    -DCMAKE_BUILD_TYPE=Release                   \
    -DLLVM_DIR=/usr/lib/llvm-15/lib/cmake/llvm   \
    -DClang_DIR=/usr/lib/llvm-15/lib/cmake/clang
```
