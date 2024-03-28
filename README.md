# The M86 LLVM Backend

Welcome to M86!

This repository contains the source code for M86 (Meliorated x86) architecture for LLVM backend.
The development is divided into several conventional stages of adding your own architecture
to the LLVM project.

Start of development [commit](https://github.com/sfbakturin/llvm-m86/commit/74e6519ee2427443528a7b7ccd402b3278b9f6b2).

List of stages:

* Register architecture.

## Getting the Source Code and Building LLVM

Consult the
[Getting Started with LLVM](https://llvm.org/docs/GettingStarted.html#getting-the-source-code-and-building-llvm)
page for information on building and running LLVM.

For M86 target you should define `LLVM_TARGETS_TO_BUILD` to `M86` when building with CMake.
