CXX=clang++ -std=c++17
CXX_FLAG=-I includes
WASM= -Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined -Wl,--import-memory --target=wasm32 -fno-builtin --no-standard-libraries
file=make
build: gen
	mkdir -p bin
	make -f ${file}

gen: src/* gen.sh
	./gen.sh "${CXX}" "${CXX_FLAG}" "${WASM}" "${file}"

test: src/basic.cc Makefile includes/*
	${CXX} ${CXX_FLAG} -DDEBUG src/basic.cc	 -o $@ 


