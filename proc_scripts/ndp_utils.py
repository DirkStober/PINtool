import os,sys




class BenchRun:
    avg_tlb_ratio = None
    min_tlb_ratio = None
    avg_p_ratio= None
    min_p_ratio= None
    def __init__(self, input_string):
        if input_string != None:
            self.read_input(input_string)

    def read_input(self,input_string):
        # split input by lines
        lines = input_string.split('\n')
        m_l = lines[1]
        d_l = lines[3:-1]
        meta = m_l.split(',')
        self.num_mem = int(meta[0])
        self.t_p_m = int(meta[1])
        self.page_size = int(meta[2])
        self.tlb_entries= int(meta[3])
        self.asso = meta[4]
        self.page_distro = meta[5]
        self.benchmark = meta[6]
        self.num_threads = self.num_mem * self.t_p_m
        self.tlb_hit=[]
        self.tlb_miss= []
        self.tlb_ratio = []
        self.p_local= []
        self.p_nlocal = []
        self.p_ratio = []
        for i in range(0,self.num_threads):
            t_l = d_l[i].split(',')
            self.tlb_hit.append(float(t_l[1]))
            self.tlb_miss.append(float(t_l[2]))
            self.tlb_ratio.append(float(t_l[3]))
            self.p_local.append(float(t_l[4]))
            self.p_nlocal.append(float(t_l[5]))
            self.p_ratio.append(float(t_l[6]))
        # Append average as well
        self.avg_tlb_ratio = sum(self.tlb_ratio)/self.num_threads
        self.min_tlb_ratio = min(self.tlb_ratio)
        self.avg_p_ratio = sum(self.p_ratio)/self.num_threads
        self.min_p_ratio = min(self.p_ratio)









class Run_file:
    bench_runs = []
    def __init__(self,file):
        if file != None:
            self.read_file(file)
    def read_file(self,file):
        in_s = open(file,'r')
        r_strings = in_s.read().split('#mem')
        r_strings.remove('')
        for el in r_strings:
            b1 = BenchRun(el)
            self.bench_runs.append(b1)

