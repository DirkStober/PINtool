import os,sys

from ndp_utils import *



rf = Run_file("../output/sssp_pa.csv")
for br in  rf.bench_runs:
    print("Tlb ratio: " + str(br.avg_tlb_ratio) + " PR: "
            + str(br.avg_p_ratio))

