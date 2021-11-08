#!/bin/bash
PIN_ROOT=PIN/pin-3.17-98314-g0c048d619-gcc-linux
PINTOOL=obj/obj-intel64/pintool.so
BENCH_RUNS="utils/ndp_run.sh";

# convert trailing sizes to integers (k,M,G)
conv_pagesize() { case ${1: -1} in
		"k") ((res=${1%k} * 1024));;
		"M") ((res=${1%M} * 1024 * 1024));;
		"G") ((res=${1%G} * 1024 * 1024 * 1024));;
		*) res=$1;;
	esac
	echo $res;
}

Usage() {
	# Usage of run script
	echo " Script to run tests using ndp pintool";
	echo " 4 Input arguments: input file,  output file, benchmark; problem size";
	echo -e "\n for input files one line indicates a run use 
	# to comment a line.
	Input lines are structured as: 
	#mem #t/mem page_size tlb_entries tlb_asso problem_size distro blocks_per_page"
}

# Inputs: Input file , benchmark , output file
[[ $# -lt 4 ]] && Usage && exit 0;

IN_FILE=$1;
OUT_FILE=$2;
BENCH_NM=$3;
PROBLEM_SIZE=$4;

echo -e "Running "$BENCH_SH" benchmark,\n using "$IN_FILE" as input and writing results to "$OUT_FILE"."; 

cn=0;
FILTER_SL="-filter_no_shared_libs";
source $BENCH_RUNS
while read line; do
	[[ -z $line ]] && continue
	[[ ${line:0:1} == "#" ]] && continue
	l=($line);
	NM=${l[0]};
	TPM=${l[1]};
	P_SIZE=$(conv_pagesize ${l[2]});
	T_SIZE=${l[3]};
	T_ASSO=${l[4]};
	D_POLICY=${l[5]};
	[[ -n ${l[6]} ]] && BLOCKS_PP="-bpp ${l[6]}";
	PIN_FLAGS="${FILTER_SL} -o "${OUT_FILE}"_"${cn}" -p ${P_SIZE} -tn ${T_SIZE} 
	-tlb_asso ${T_ASSO} -nm ${NM} -tpm ${TPM} -pd ${D_POLICY} ${BLOCKS_PP}";
	NUM_THREADS=$(( NM * TPM));
	export OMP_NUM_THREADS=$(( NM * TPM));
	# Call correct script with command line input and etc..
	CMD=$(run_bench $BENCH_NM);
	echo $CMD;
	$CMD > ${OUT_FILE}_${cn}.log;
	[[ -n "$5" ]] && \
		scp  ${OUT_FILE}_${cn} $5 && \
		scp  ${OUT_FILE}_${cn}.log $5 && \
		rm   ${OUT_FILE}_${cn} && \
		rm   ${OUT_FILE}_${cn}.log
	let cn+=1;
done < $IN_FILE
#Clear log files of hpcg benchmarks
[[ $3 == "hpcg" ]] && rm -f hpcg*.txt;
echo "Done !"
