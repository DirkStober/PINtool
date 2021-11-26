import os,sys
import numpy as np
import matplotlib.pyplot as plt


class BenchRun:
    def __init__(self, input_string):
        self.avg_tlb_ratio = None
        self.min_tlb_ratio = None
        self.avg_p_ratio= None
        self.min_p_ratio= None
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
        self.asso = int(meta[4])
        self.page_distro = int(meta[5])
        self.blocks_per_page= int(meta[6])
        self.ppd_int = self.page_distro * self.blocks_per_page
        self.benchmark = meta[7]
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
    def __init__(self,file):
        self.page_sizes = []
        self.bench_runs = []
        self.benchmark = ""
        self.problem_size = ""
        if file != None:
            self.file = file
            self.read_file(file)
            self.sort_benchmarks()


    def read_file(self,file):
        in_s = open(file,'r')
        r_strings = in_s.read()
        r_strings = r_strings.split('#mem')
        r_strings.remove('')
        for el in r_strings:
            b1 = BenchRun(el)
            self.bench_runs.append(b1)
        bm = self.bench_runs[0].benchmark.split()[0]
        bm = bm.split("/")[-1]
        self.benchmark = bm
        ps = self.bench_runs[0].benchmark.split()[1:]
        # This is for filtering porposes and the last value indicates
        # the problem size
        self.problem_size = ps[-1]
        in_s.close()


    #Sort the benchmarks by page sizes and so on
    # The runs are sorted by TLB asso 
    # and then their data is stored in arrays with increasing page size
    def sort_benchmarks(self):
        # Find out what kind of assos there are
        assos = []
        page_distros = []
        page_sizes = []
        self.tlb_entries = self.bench_runs[0].tlb_entries
        self.num_mem = self.bench_runs[0].num_mem
        self.t_p_m = self.bench_runs[0].t_p_m
        for b in self.bench_runs:
            if(b.tlb_entries != self.tlb_entries):
                print("Tlb size change within benchmarks !")
            if(b.num_mem != self.num_mem):
                print("Num memories change within benchmarks !")
            if(b.t_p_m!= self.t_p_m):
                print("Threads/mem size change within benchmarks !")
            if not(b.asso in assos):
                assos.append(b.asso)
            pd = int(b.page_distro) * int(b.blocks_per_page)
            if not(pd in page_distros):
                page_distros.append(pd)
            ps = b.page_size
            if not(ps in page_sizes):
                page_sizes.append(ps)
        assos.sort()
        page_distros.sort()
        page_sizes.sort()
        self.tlb_assos = assos
        self.page_distros = page_distros
        self.page_sizes = page_sizes
        # PS/Asso : 4           8       16 ...
        # 1       : avg/min     avg/min avg/min
        # 2       :
        # 4       :
        self.avg_tlb = [[None]*len(self.page_sizes)  \
                for i in range(len(self.tlb_assos))]
        self.min_tlb = [[None]*len(self.page_sizes) \
                for i in range(len(self.tlb_assos))]
        self.avg_pd =[[None]*len(self.page_sizes) \
                for i in range(len(self.page_distros))]
        self.min_pd=[[None]*len(self.page_sizes)
                for i in range(len(self.page_distros))]
        # Actually start sorting the benchmarks
        for br in self.bench_runs:
            # TLB size should not be changed for the entire run
            ta = br.asso
            # First Touch is 0 RR is 1, so pd * bpp is unique for 
            # each combination 
            pd = int(br.page_distro) * int(br.blocks_per_page)
            # Page index:
            p_i = self.page_sizes.index(br.page_size)
            #First do TLB asso:
            ta_i = self.tlb_assos.index(ta)

            if self.avg_tlb[ta_i][p_i] == None:
                self.avg_tlb[ta_i][p_i] = br.avg_tlb_ratio
                self.min_tlb[ta_i][p_i] = br.min_tlb_ratio
            #if another benchmark already occupy the space
            else:
                print("Multiple values for tlb asso: " + str(ta) + \
                        "picking result with smaller avergae hit rate!")
                print("asso: " + str(ta) + " ps: " + str(br.page_size)) 
                print("bm: " + br.benchmark) 
                if self.avg_tlb[ta_i][p_i] > br.avg_tlb_ratio:
                    self.avg_tlb[ta_i][p_i] = br.avg_tlb_ratio
                    self.min_tlb[ta_i][p_i] = br.min_tlb_ratio
            # Same for local page rate
            pd_i = self.page_distros.index(pd)
            if self.avg_pd[pd_i][p_i] == None:
                self.avg_pd[pd_i][p_i] = br.avg_p_ratio
                self.min_pd[pd_i][p_i] = br.min_p_ratio
            #if another benchmark already occupy the space
            else:
                print("Multiple values for p asso: " + str(pd) + \
                        "picking result with smaller avergae hit rate!")
                print("distro: " + str(pd) + " ps: " + str(br.page_size)) 
                if self.avg_pd[pd_i][p_i] > br.avg_p_ratio:
                    self.avg_pd[pd_i][p_i] = br.avg_p_ratio
                    self.min_pd[pd_i][p_i] = br.min_p_ratio


    """ Print the results into a csv table """
    def get_csv_string(self):
        rs = "\n\n File:," + self.file + "\n"
        rs += "TLB entries:," + str(self.tlb_entries) + '\n'
        rs += "NM/TPM:," + str(self.num_mem) + "/" + str(self.t_p_m) + "\n"
        rs += ",Page Size:"
        for p in self.page_sizes:
            rs+= "," + str(int(p/1024)) + "kB"
        rs += "\nAsso:\n"
        for i in range(0,len(self.tlb_assos)):
            avg_s= str(self.tlb_assos[i]) + ",AVG"
            min_s= str(self.tlb_assos[i]) + ",MIN"
            for j in range(0,len(self.page_sizes)):
                avg_s += "," + str(self.avg_tlb[i][j])
                min_s += "," + str(self.min_tlb[i][j])
            rs += avg_s + "\n" + min_s + "\n"
        rs += ",Page Size:"
        for p in self.page_sizes:
            rs+= "," + str(int(p/1024)) + "kB"
        rs += "\n Pd: \n"
        for i in range(0,len(self.page_distros)):
            if self.page_distros[i] == 0:
                pds = "FT"
            else:
                pds = "RR " + str(self.page_distros[i])
            avg_s=pds +  ",AVG"
            min_s=pds + ",MIN"
            for j in range(0,len(self.page_sizes)):
                avg_s += "," + str(self.avg_pd[i][j])
                min_s += "," + str(self.min_pd[i][j])
            rs += avg_s + "\n" + min_s + "\n"
        return rs

def conv_pages(in_s):
    r_s = []
    for i in in_s:
        r = 0
        KB=1024
        MB= KB*1024
        GB= MB*1024
        if(i/GB >= 1):
            r_s.append(str(int(i/GB)) + "G")
        elif(i/MB >=1):
            r_s.append(str(int(i/MB)) + "M")
        elif(i/KB >= 1):
            r_s.append(str(int(i/KB)) + "k")
        else:
            r_s.append(str(int(i)) +"")
    return r_s

def conv_pages_inv(in_pages):
    po = []
    i = 0
    for p in in_pages:
        po.append(in_pages[i])
        if isinstance(p,str):
            f = p[-1]
            val = int(p[:-1])
            if(f == "k"):
                po[i] = val * 1024
            elif f == "M":
                po[i] = val * 1024 * 1024
            elif f == "G": 
                po[i] = val * 1024 *1024*1024
        i =  i+1
    return po




def plot_runs(runs,graph_dir):
    plot_colors = ['b','g','r','y','m','k','c']
    fig = plt.figure(0)
    #subfigs = fig.subfigures(len(runs) + 1,1,wspace = 0.07)
    subfigs = []
    avg_tlb = [[0.0]*len(runs[0].page_sizes)  \
                for i in range(len(runs[0].tlb_assos))]
    min_tlb = [[0.0]*len(runs[0].page_sizes)  \
                for i in range(len(runs[0].tlb_assos))]
    avg_pd = [[0.0]*len(runs[0].page_sizes)  \
                for i in range(len(runs[0].tlb_assos))]
    min_pd = [[0.0]*len(runs[0].page_sizes)  \
                for i in range(len(runs[0].tlb_assos))]
    for i in range(len(runs)):
        subfigs.append(plt.figure(i))
        subfigs[i].suptitle('File: ' + runs[i].file)
        axs = subfigs[i].subplots(1, 2)
        r = runs[i]
        axs[0].set_title("Avg and minimal TLB hit Ratio")
        axs[1].set_title("Avg and minimal page local Ratio")
        axs[0].set_xlabel("Page Size")
        axs[1].set_xlabel("Page Size")
        axs[0].set_ylabel("Tlb hit Ratio")
        axs[1].set_ylabel("Page Local Ratio")
        for j in range(len(r.tlb_assos)):
            col=plot_colors[j]
            l_tlb = str(r.tlb_assos[j])
            l_p = str(r.page_distros[j])
            axs[0].plot(conv_pages(r.page_sizes),r.avg_tlb[j],\
                  "-" + str(col),label = l_tlb)
            axs[0].plot(conv_pages(r.page_sizes),r.min_tlb[j],\
                  "-." + str(col),label = l_tlb)
            axs[0].legend(loc="lower right")
            axs[1].plot(conv_pages(r.page_sizes),r.avg_pd[j],\
                  "-" + str(col), label = l_p)
            axs[1].plot(conv_pages(r.page_sizes),r.min_pd[j],\
                  "--" + str(col) ,label = l_p)
            axs[0].legend(loc="lower right")
            axs[1].legend(loc="upper right")
            for p in range(len(r.page_sizes)):
                if(r.avg_pd[j][p] != None):
                    avg_pd[j][p] += r.avg_pd[j][p]/len(runs)
                    min_pd[j][p] += r.min_pd[j][p]/len(runs)
                else:
                    avg_pd[j][p] = None
                    min_pd[j][p] = None
                if(r.avg_tlb[j][p] != None):
                    avg_tlb[j][p] += r.avg_tlb[j][p]/len(runs)
                    min_tlb[j][p] += r.min_tlb[j][p]/len(runs)
                else:
                    avg_tlb[j][p] = None
                    min_tlb[j][p] = None
                
    subfigs.append(plt.figure(len(runs)))
    axs = subfigs[len(runs)].subplots(1,2)
    subfigs[len(runs)].suptitle("Avg avg and minimum values for all files")
    for j in range(len(runs[0].tlb_assos)):
        l_tlb = str(r.tlb_assos[j])
        l_p = str(r.page_distros[j])
        col = plot_colors[j]
        axs[0].plot(conv_pages(runs[0].page_sizes),avg_tlb[j],\
              "-" + str(col),label = l_tlb)
        axs[0].plot(conv_pages(runs[0].page_sizes),min_tlb[j],\
              "-." + str(col),label = l_tlb)
        axs[0].legend(loc="lower right")
        axs[1].plot(conv_pages(runs[0].page_sizes),avg_pd[j],\
              "-" + str(col), label = l_p)
        axs[1].plot(conv_pages(runs[0].page_sizes),min_pd[j],\
              "--" + str(col) ,label = l_p)
        axs[0].legend(loc="lower right")
        axs[1].legend(loc="upper right")
    i = 0
    for sf in subfigs:
        if(i == len(runs)):
            file_s = "average"
        else:
            file_s = str(runs[i].file).split('/')[-1]
        sf.savefig(graph_dir + "/fig_" + file_s + ".eps")
        i+=1
    os.system("gs -sDEVICE=pdfwrite -dNOPAUSE -dBATCH -sOutputFile='" + graph_dir +"/sum.pdf' " + graph_dir + "/*.eps")










