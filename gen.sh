#!/usr/bin/bash
CXX=$1
CXX_FLAG=$2
WASM=$3
file=$4
echo -n "" > ${file}
echo -n "all: " >> ${file}
# echo -e -n "\t" >> ${file}
for entry in src/*; do
    name=${entry/\//}
    name=${name/src/}
    name=${name/.cc/}
    wasm=index/${name}.wasm

    echo -n ${wasm} " " >> ${file}
done
echo "" >> ${file}
for entry in src/*; do
    name=${entry/\//}
    name=${name/src/}
    name=${name/.cc/}
    wasm=index/${name}.wasm

    echo ${wasm}: ${entry} "includes/*" >> ${file}
    echo -e '\t'${CXX} ${CXX_FLAG} ${WASM} ${entry} "-o" ${wasm} >> ${file}

    
done
