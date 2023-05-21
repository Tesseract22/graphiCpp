CXX=clang++
CXX_FLAG=
WASM= -Wl,--no-entry -Wl,--export-all --target=wasm32 -fno-builtin --no-standard-libraries 

all: index.wasm test

index.wasm: src/main.cc Makefile
	${CXX} ${CXX_FLAG} ${WASM}  src/main.cc	 -o $@ 

test: src/main.cc Makefile
	${CXX} ${CXX_FLAG} -DDEBUG src/main.cc	 -o $@ 