# graphiCpp
This is a immediate graphical library written in C++. Works completely on CPU and can be compiled to standalone wasm.
See the wasm demos at https://tesseract22.github.io/graphiCpp/.

All this library does is taking a buffer and draw shapes on that buffer, and a minimal toolchain for the buffer to display on browser (wasm) and terminal (ascii). 

This is a powerful tool for integrating small, quick, costumizable visualization for existing C++ project.

## Installation & Build
`git clone https://github.com/Tesseract22/graphiCpp.git && cd graphiCpp`

To build static an dynamic lib, 

`make lib`

and instal with

`sudo make install`

To build all wasm demo,

`make wasm`

To build all terminal ascii demo,

`make term`

To build all tests,

`make test`

## TODO

- [ ] Multithreading
- [ ] SIMD vectorization
- [ ] Tutorial
- [ ] Better anti-alias for triangle
- [ ] Fix perspective projection of sphere
- [ ] Windows Comptiable (currently not)
- [ ] ASCII demo with `SDL2` or `nCurse`.
- [ ] Windows comptiable
- [ ] WebGPU (?)

I currently have plans to migrate to Zig, for

- better memory management
- better build system
- better meta programming


