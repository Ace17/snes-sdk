This is a cleaned-up fork of the 'snes-sdk'.

Differences from the original:

- Focus on code readability: that's the whole point of using a higher-level language like C.
- The original came with some version of snes9x. This is additional maintenance, and isn't really needed: just use your favorite SNES emulator (I'm using mednafen).

Objectives:

- Switch to a LLVM/clang toolchain: this way we will benefit from lots of code optimizations, allowing us to write simpler C code.
- C99 support. Currently, only C89 is supported.
- Basic C++ 17 support. It would be nice to have: constexpr, virtual functions, destructors, templates. On the other hand, we probably don't need: new, catch/throw, the STL.

