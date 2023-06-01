CXX=clang++
CXX_FLAG=-Iincludes
WASM= -Wl,--no-entry -Wl,--export-all --target=wasm32 -fno-builtin --no-standard-libraries 

all: basic.wasm test

basic.wasm: src/basic.cc Makefile includes/*
	${CXX} ${CXX_FLAG} ${WASM}  src/basic.cc	 -o $@ 

test: src/basic.cc Makefile includes/*
	${CXX} ${CXX_FLAG} -DDEBUG src/basic.cc	 -o $@ 