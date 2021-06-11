/*
 * Code for Tracking memory acceses of the Binaries
 * Used with PIN 3.17
 *
 * Author: Dirk Stober <dirk.stober@posteo.de>
 */



#include "pintool.h"
#include <iostream>
#include <fstream>
#include "pin.H"

//#define DEBUG_INFO 0
//#define DEBUG_NDP 1


// Inlcude both tlb and pagetrack
#include "pagetrack.h"
#include "filter_custom.H"

// Define Malloc and free
#define MALLOC "malloc"
#define FREE "free"
#define MEMALIGN "posix_memalign"


static int tracking_active;

using namespace NDP_FILTER;
FILTER filter;


using std::cout;
using std::cerr;
using std::endl;

KNOB<std::string> knob_output(KNOB_MODE_WRITEONCE,"pintool",
		"o", "ndp.out", "specify log file name");

KNOB<UINT64> knob_page_size(KNOB_MODE_WRITEONCE,"pintool",
		"p", "2048", "Page Size (bytes)");
	

KNOB<UINT32> knob_num_mems(KNOB_MODE_WRITEONCE, "pintool",
		"nm", "1", "Number of memory modules threads are equally distributed and should be a multiple of nm");

KNOB<UINT32> knob_num_threads(KNOB_MODE_WRITEONCE, "pintool",
		"tpm", "1", "Number of Threads per memory module");

KNOB<UINT32> knob_num_tlb(KNOB_MODE_WRITEONCE, "pintool",
		"tn", "32", "Number of Tlb entries");

KNOB<std::string> knob_track_func(KNOB_MODE_WRITEONCE, "pintool",
		"tf", "NDP_TRACK", "Memory accesses inside this function are tracked");

KNOB<INT32> knob_blocks_per_page(KNOB_MODE_WRITEONCE, "pintool",
		"bpp", "1" , "Number of memory blocks one page is distributed over");

KNOB<INT32> knob_page_distro(KNOB_MODE_WRITEONCE, "pintool",
		"pd", "0" , "Page distribution: 0 = First touch; 1 = Round Robin");

#define PT_FIRST_TOUCH 0
#define PT_STATIC 1
static uint64_t page_offset;







// Thread local storage 
ndp_tls  * threads_data;
ndp_params params;





NDP::PT_FT * heap_FT;
NDP::PT_S * heap_S;

static inline void memOpPage(uint64_t page, ndp_tls * tls, uint64_t r){
	uint64_t tlb_r = tls->tlb->tlb_access(page); 
	tls->tlb_hits += tlb_r & (0b1);
	tls->tlb_misses += (tlb_r >> 1);
	tls->pt_hits += r & (0b1);
	tls->pt_misses += (r  >> 1);
}

static VOID SimulateMemOpFT
(
		VOID * addr, 
		UINT32 size, 
		THREADID tid
)
{
	if(!tracking_active){
		return;
	}
	ndp_tls * tls = &threads_data[tid]; 
	uint64_t page = ((uint64_t) addr);
	page = page >> page_offset;
	uint64_t r = heap_FT->acc_page(page, tls->mem_id);
	if(r){
		memOpPage(page,tls,r);
	}
}

static VOID SimulateMemOpStatic
(
		VOID * addr, 
		UINT32 size, 
		THREADID tid
)
{
	if(!tracking_active){
		return;
	}
	ndp_tls * tls = &threads_data[tid]; 
	uint64_t r = heap_S->acc_addr((uint64_t) addr, tls->mem_id);
	if(r){
		uint64_t page = ((uint64_t) addr);
		page = page >> page_offset;
		memOpPage(page,tls,r);
	}
}



static VOID ThreadStart(THREADID tid, CONTEXT *ctxt, INT32 flags, VOID * v)
{
	printf("Thread %d started!\n",tid);
	// Calculate mem id
	int num_mems = params.nm;
	int tpm = params.tpm;
	int8_t mem_id = (tid/tpm);
	if(mem_id >= num_mems){
		std::cerr << "Invalid values for num mems or threads per mem mod!\n";
	}
	threads_data[tid].mem_id = mem_id;
}


static VOID ThreadFini(THREADID tid, const CONTEXT *ctxt, INT32 flags, VOID * v)
{
	printf("Thread %d finished!\n",tid);	
}





static VOID Trace(TRACE trace, VOID * val)
{
	if(!filter.SelectTrace(trace))
		return;



	for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
	{
    		for (INS ins = BBL_InsHead(bbl); INS_Valid(ins); ins = INS_Next(ins))
    		{
    			UINT32 memOperands = INS_MemoryOperandCount(ins);
    			for(UINT32 memOp =0; memOp < memOperands; memOp++){
    				UINT32 size = INS_MemoryOperandSize(ins,memOp);
				AFUNPTR SimulateMemOp;
				if(params.page_distro == PT_FIRST_TOUCH){
					SimulateMemOp = (AFUNPTR) SimulateMemOpFT;
				}
				else{
					SimulateMemOp = (AFUNPTR) SimulateMemOpStatic;
				}
    				INS_InsertCall(ins, IPOINT_BEFORE, 
    					(AFUNPTR) SimulateMemOp,
    					IARG_MEMORYOP_EA, memOp,
    					IARG_UINT32, size,
    					IARG_THREAD_ID,
    					IARG_END);
    			}

    		}
    	    
    	}
}



static VOID MallocBefore(ADDRINT size, THREADID tid)
{
	if(tid != 0)
		return;
	threads_data[tid]._malloc_size = (uint64_t) size;
}

static VOID MallocAfter( ADDRINT ret, THREADID tid)
{
	if(tid != 0)
		return;
	// add mem block !!
	uint64_t addr = (uint64_t) ret;
	uint64_t size = threads_data[tid]._malloc_size;
	uint64_t start,stop;
	if(params.page_distro == PT_FIRST_TOUCH){
		start = (addr) >> page_offset;
		stop = (addr + size - 1) >> page_offset;
		heap_FT->add_memblock(start,stop);
	}
	else{
		start = addr;
		stop = (addr+ size -1);
		heap_S->add_memblock(start,stop);
	}


#ifdef DEBUG_INFO
	printf("MALLOC TID : %d \n" , tid);
	printf("addr: %lu ;size: %lu \n",addr,size);
	printf("start: %lu ;stop: %lu \n",start,stop);
#endif

}

static VOID TrackBefore(THREADID tid){
	if(tid != 0){
		printf("HELLO from  %d\n",tid);
		return;
	}
	tracking_active = 1;
	return;
}


static VOID TrackAfter(THREADID tid){
	if(tid != 0){
		printf("HELLO from  %d\n",tid);
		return;
	}
	tracking_active = 0;
	return;
}

static void * memal;
static uint64_t msize;
static VOID MEMALIGNBefore(THREADID tid, void * ptr, ADDRINT size){
	if(tid != 0)
		return;
	memal = ptr;
	msize = (uint64_t) size;
}


static VOID MEMALIGNAfter(THREADID tid){
	if(tid != 0)
		return;
	
	uint64_t addr =*(uint64_t *) memal;
	uint64_t size = msize;
	uint64_t start,stop;
	if(params.page_distro == PT_FIRST_TOUCH){
		start = (addr) >> page_offset;
		stop = (addr + size - 1) >> page_offset;
		heap_FT->add_memblock(start,stop);
	}
	else{
		start = addr;
		stop = (addr+ size -1);
		heap_S->add_memblock(start,stop);
	}
#ifdef DEBUG_INFO
	printf("MEMALIGN TID : %d \n" , tid);
	printf("addr: %lu ;size: %lu \n",addr,size);
	printf("start: %lu ;stop: %lu \n",start,stop);
#endif
}




/**
 * Checks wether the called routine is MALLOC or free
 * if so adds a memory block to it tracking the pages
 *
**/
static VOID IMAGE(IMG img, VOID *v)
{	
	// On uu machine this image performs extra mem allocations
	// which dont occur on arch machine.
	// It is thus excluded 
	// TODO: Look into this !
	if(IMG_Name(img) == "/lib64/ld-linux-x86-64.so.2"){
		return;
	}
	RTN mallocRtn = RTN_FindByName(img, MALLOC);
	if (RTN_Valid(mallocRtn))
	{
		RTN_Open(mallocRtn);
		
		// Instrument malloc() to print the input argument value and the return value.
		RTN_InsertCall(mallocRtn, IPOINT_BEFORE, (AFUNPTR)MallocBefore,
		               IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
			       IARG_THREAD_ID,
		               IARG_END);
		RTN_InsertCall(mallocRtn, IPOINT_AFTER, (AFUNPTR)MallocAfter,
		               	IARG_FUNCRET_EXITPOINT_VALUE, 
				IARG_THREAD_ID,
				IARG_END);
		
		RTN_Close(mallocRtn);
	}
	
	for( SEC sec= IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec) ){
		for( RTN rtn= SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn) ){
			if(PIN_UndecorateSymbolName(RTN_Name(rtn),UNDECORATION_NAME_ONLY) == knob_track_func.Value()){
				RTN_Open(rtn);
				RTN_InsertCall(rtn, IPOINT_BEFORE, (AFUNPTR)TrackBefore,
					       IARG_THREAD_ID,
				               IARG_END);
				RTN_InsertCall(rtn, IPOINT_AFTER, (AFUNPTR)TrackAfter,
						IARG_THREAD_ID,
						IARG_END);
				RTN_Close(rtn);
			}
			if(PIN_UndecorateSymbolName(RTN_Name(rtn),UNDECORATION_NAME_ONLY) == MEMALIGN){
				RTN_Open(rtn);
				RTN_InsertCall(rtn, IPOINT_BEFORE, (AFUNPTR)MEMALIGNBefore,
					       IARG_THREAD_ID,
					       IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
					       IARG_FUNCARG_ENTRYPOINT_VALUE, 2,
				               IARG_END);
				RTN_InsertCall(rtn, IPOINT_AFTER, (AFUNPTR)MEMALIGNAfter,
						IARG_THREAD_ID,
						IARG_END);
				RTN_Close(rtn);
			}
		}
	};

}

INT32 Usage(){
	cerr << "Tool to analyze NDP benchmarks \n ";
        cerr << KNOB_BASE::StringKnobSummary() << endl;
	return -1;
};




VOID Fini(INT32 code, VOID *v){
	int nm = knob_num_mems.Value();
	int tpm = knob_num_threads.Value();
	write_file(&params,knob_output.Value().c_str());
	print_output(&params);
	
	for(int i = 0 ; i < tpm*nm; i++){
		delete threads_data[i].tlb;
	}
	delete threads_data;
	if(params.page_distro == PT_FIRST_TOUCH)
		delete heap_FT;
	else{
		delete heap_S;
	}
}



int init_tls(int nm , int tpm)
{
	for(int i = 0; i < nm * tpm; i++){
		threads_data[i].tlb = new TLB(params.tlb_entries);
		threads_data[i].mem_id = (i/tpm);
	}
	return 0;
}
	

	


int main(int argc, char * argv[])
{

	PIN_InitSymbols();
	if(PIN_Init(argc,argv)){
		return Usage();
	}
	// Init parameters for run
	params.page_size = knob_page_size.Value();
	params.tlb_entries = knob_num_tlb.Value();
	params.nm = (int) knob_num_mems.Value();
	params.tpm = knob_num_threads.Value();
	// Allocate thread local storage
	threads_data = new ndp_tls[params.nm*params.tpm];
	// Store pointer to array in parameters
	params.threads_data = threads_data;
	params.page_distro = knob_page_distro.Value();
	int8_t num_mems = knob_num_mems.Value();

	print_info(&params);
	init_tls(num_mems,params.tpm);

	// Allocate new pagetrack
	if(params.page_distro == PT_FIRST_TOUCH){
		heap_FT= new NDP::PT_FT(num_mems);
	}
	else if(params.page_distro == PT_STATIC){
		heap_S = new NDP::PT_S(num_mems,params.page_size,knob_blocks_per_page.Value());
	}
	else{
		fprintf(stderr,"Not a valid page distro !%d \n", params.page_distro);
		return 1;
	}
	page_offset = log_2_uint64_t(params.page_size);
	filter.Activate();
	TRACE_AddInstrumentFunction(Trace,0);
	IMG_AddInstrumentFunction(IMAGE,0);
	
	
	PIN_AddThreadStartFunction(ThreadStart, 0);
	PIN_AddThreadFiniFunction(ThreadFini,0);
	PIN_AddFiniFunction(Fini,0);

	// Never Return 
	PIN_StartProgram();
	return 0;
}


