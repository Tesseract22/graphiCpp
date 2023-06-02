CXX=clang++
CXX_FLAG=-I includes
WASM= -Wl,--no-entry -Wl,--export-all --target=wasm32 -fno-builtin --no-standard-libraries 

demos := basic

gen: src/* gen.sh
	./gen.sh "${CXX}" "${CXX_FLAG}" "${WASM}"

all: basic.wasm test

basic.wasm: src/basic.cc Makefile includes/*
	${CXX} ${CXX_FLAG} ${WASM}  src/basic.cc	 -o $@ 

color.wasm: src/color.cc Makefile includes/*




test: src/basic.cc Makefile includes/*
	${CXX} ${CXX_FLAG} -DDEBUG src/basic.cc	 -o $@ 


