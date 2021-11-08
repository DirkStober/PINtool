#!/bin/sh
TF="main";
FILTER_RTN="-filter_rtn do_work";

GRAPH_TF="main"
GRAPH_RTN="-filter_rtn do_work";
GRAPH_S="-tf ${GRAPH_TF} ${GRAPH_RTN}"; 

#General extension for graph algorithms to get proper inputs
graph_exp() {
	local input="$@";
	echo ""$2" "$1" ${input#* * }"; 
}

GRAPH_PARAMS=(
	"0 1000 10"
	"1 benchmarks/data/roadNet-PA.txt"
	"1 benchmarks/data/roadNet-TX.txt"
	"1 benchmarks/data/roadNet-CA.txt"
	"1 benchmarks/data/USA-road-d.LKS.gr"
);

## Breath-First Search
BFS_BIN=benchmarks/bfs/bfs
bfs_run(){
	local PARAMS=${GRAPH_PARAMS[${PROBLEM_SIZE}]};
	echo "$GRAPH_S -- $BFS_BIN "$(graph_exp $NUM_THREADS $PARAMS)""
}

## PageRank
PR_BIN=benchmarks/pagerank/pagerank
pr_run(){
	local PARAMS=${GRAPH_PARAMS[$PROBLEM_SIZE]};
	echo "$GRAPH_S -- $PR_BIN "$(graph_exp $NUM_THREADS $PARAMS)"";
}

## Single Source Shortest Path (SSSP)
SSSP_BIN=benchmarks/sssp/sssp
sssp_run(){
	local PARAMS=${GRAPH_PARAMS[$PROBLEM_SIZE]};
	echo "$GRAPH_S -- $SSSP_BIN "$(graph_exp $NUM_THREADS $PARAMS)"";
}


## HPCG
HPCG_BIN=benchmarks/hpcg-3.1/NDP/bin/ndpcg
HPCG_S="-tf CG"
HPCG_PARAMS=(
	"16 16 16"
	"104 104 104"
	"208 208 208"
);
hpcg_run(){
	echo "$HPCG_S -- $HPCG_BIN ${HPCG_PARAMS[$PROBLEM_SIZE]} "
}


## TEST
test_run(){
	echo "-tf main -- benchmarks/test/test"
}


## Function called by main run script NUM_THREADS specifies the number
# of threads to execute with PROBLEM_SIZE the problem number
# $1 is a string specifying the benchmark
run_bench() {
	case $1 in
		"bfs") RET=$(bfs_run);;	
		"hpcg") RET=$(hpcg_run);;
		"pagerank"|"pr") RET=$(pr_run);;
		"sssp") RET=$(sssp_run);;
		"test") RET=$(test_run);;
		*) echo "$1 not implemented!" && exit 0;;
	esac;
	CMD="${PIN_ROOT}/pin -t ${PINTOOL} ${PIN_FLAGS} "
	CMD+=" $RET"
	echo $CMD;
}
