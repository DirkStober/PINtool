#!/bin/bash
BIN=PIN/pin-3.17-98314-g0c048d619-gcc-linux/pin 
TOOL=" -t obj/obj-intel64/pintool.so "
FLAGS=" -filter_no_shared_libs -p 4096 -tn 32 -tlb_asso 1 -nm 4 -tpm 2 -pd 0 -bpp 1 -tf main  "


BM=benchmarks/sssp
PS=1

GRAPH_PARAMS=(
	" benchmarks/data/roadNet-PA.txt"
	" benchmarks/data/roadNet-TX.txt"
	" benchmarks/data/roadNet-CA.txt"
);
benchmarks/sssp/sssp 1 8 benchmarks/data/roadNet-PA.txt
${BIN} ${TOOL} ${FLAGS} -o output/test.out -footprint 1 -- \
	${BM} 1 8  GRAPH_PARAMS[0]  
