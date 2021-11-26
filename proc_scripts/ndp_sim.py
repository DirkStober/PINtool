import os,sys
from ndp_utils import Run_file

def fatr(in_r, attr, value):
    rets = []
    for r in in_r:
        if(getattr(r,attr) in  value):
            rets.append(r)
    return rets

default = {
        'freq' : 3*(1000 * 1000 * 1000),
        't_tlb' : [1,100],
        't_dl' : [1,50]
        }


def sim(bms,asso,pd,freq=default['freq'],t_tlb = default['t_tlb'], t_dl = default['t_dl'] ):
    # Get the benchmarks
    tlb_bench = fatr(bms,'asso',[asso])[0]
    dl_bench= fatr(bms,'ppd_int',[pd])[0]
    t = []
    for i in range(len(tlb_bench.tlb_hit)):
        t.append(t_tlb[0] * tlb_bench.tlb_hit[i] + t_tlb[1] * tlb_bench.tlb_miss[i] \
                + t_dl[0]*dl_bench.p_local[i] + t_dl[1] * dl_bench.p_nlocal[i])
    t_avg = (sum(t) / len(t))/freq
    t_max = max(t) /freq
    return [t_avg,t_max]





if __name__ == "__main__":
    r = Run_file(sys.argv[1])
    for b in r.bench_runs:
        print(sim(b,freq=1024*1024))

