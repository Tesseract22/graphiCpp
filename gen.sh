#!/usr/bin/bash
CXX=$1
CXX_FLAG=$2
WASM=$3

echo -n "" > file
for entry in src/*; do
    name=${entry/\//}
    name=${name/src/}
    name=${name/.cc/}

    wasm=${name}.wasm
    echo ${wasm}: ${entry} "includes/*" >> file
    echo -e '\t'${CXX} ${CXX_FLAG} ${WASM} ${entry} "-o" ${wasm} >> file
done
