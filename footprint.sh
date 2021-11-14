#!/bin/bash
BIN=PIN/pin-3.17-98314-g0c048d619-gcc-linux/pin 
TOOL=" -t obj/obj-intel64/pintool.so "



GRAPH_TF="main"
GRAPH_RTN="-filter_rtn do_work";
GRAPH_S="-tf ${GRAPH_TF} ${GRAPH_RTN}"; 


FLAGS=" -filter_no_shared_libs -p 4096 -tn 32 -tlb_asso 1 -nm 4 -tpm 2 -pd 0 -bpp 1 "

GRAPH_PARAMS=(
	" benchmarks/data/roadNet-PA.txt"
	" benchmarks/data/roadNet-TX.txt"
	" benchmarks/data/roadNet-CA.txt"
);

BMS=(
	"benchmarks/sssp/sssp"
	"benchmarks/bfs/bfs"
	"benchmarks/pagerank/pagerank"
);

p="-pause_tool 20"
#${BIN}   ${TOOL} ${FLAGS} -tf do_work -o output/fp.out -footprint 1 -- benchmarks/test/test 





for bm in "${BMS[@]}"
do
for i in {0..2}
do
	out_f="output/fp_${bm##*/}_$i.log"
	echo "Running ${bm} with ps: $i storing in ${out_f}"
	echo "${BIN} ${TOOL} ${FLAGS} ${GRAPH_S} -o output/fp.out -footprint 1 --  ${bm} 1 8  ${GRAPH_PARAMS[i]} > ${out_f}"
	${BIN} ${TOOL} ${FLAGS} ${GRAPH_S} -o output/fp.out -footprint 1 --  ${bm} 1 8  ${GRAPH_PARAMS[i]} > ${out_f}
	[[ -n "$1" ]] && \
		scp  ${out_f} $1 && \
		rm   ${out_f}
done
done

