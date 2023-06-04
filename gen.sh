#!/usr/bin/bash
CXX=$1
CXX_FLAG=$2
WASM=$3
file=$4

wasm_path=docs
echo -n "" > ${file}
echo -n "all: " >> ${file}
# echo -e -n "\t" >> ${file}
for entry in src/*; do
    name=${entry/\//}
    name=${name/src/}
    name=${name/.cc/}
    wasm=${wasm_path}/${name}.wasm

    echo -n ${wasm} " " >> ${file}
    echo -n bin/${name} " " >> ${file}
done
echo "" >> ${file}
for entry in src/*; do
    name=${entry/\//}
    name=${name/src/}
    name=${name/.cc/}
    wasm=${wasm_path}/${name}.wasm

    echo ${wasm}: ${entry} "includes/*" >> ${file}
    echo -e '\t'${CXX} ${CXX_FLAG} ${WASM} ${entry} "-o" ${wasm} >> ${file}

    echo bin/${name}: ${entry} "includes/*" >> ${file}
    echo -e '\t'${CXX} ${CXX_FLAG} ${entry} "-DDEBUG -o" bin/${name} >> ${file}

    
done

