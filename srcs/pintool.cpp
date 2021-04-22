/*
 * Code for Tracking memory acceses of the Binaries
 * Used with PIN 3.17
 *
 * Author: Dirk Stober <dirk.stober@posteo.de>
 */



#include <iostream>
#include <fstream>
#include "pin.H"


//#define DEBUG_INFO 0


// Inlcude both tlb and pagetrack
#include "tlb.h"
#include "pagetrack.h"

// Define Malloc and free
#define MALLOC "malloc"
#define FREE "free"


static uint64_t log_2_uint64_t(uint64_t a){
	uint64_t result = 0;
	while(a >>= 1){
		result++;
	}
	return result;
}

static int page_offset;


// Use a lock for access to allocate mem pages
PIN_RWMUTEX mem_block_lock;


// Use a start and stop function to track memory accesses 
#define NDP_START_STOP_FUNCTION "_NDP_PIN_START_STOP_"


using std::cout;
using std::cerr;
using std::endl;

KNOB<std::string> knob_output(KNOB_MODE_WRITEONCE,"pintool",
		"o", "ndp.out", "specify log file name");

KNOB<UINT64> knob_page_size(KNOB_MODE_WRITEONCE,"pintool",
		"p", "2048", "Page Size (bytes)");
static int ndp_page_size;
	

KNOB<UINT32> knob_num_mems(KNOB_MODE_WRITEONCE, "pintool",
		"nm", "1", "Number of memory modules threads are equally distributed and should be a multiple of nm");

KNOB<UINT32> knob_num_threads(KNOB_MODE_WRITEONCE, "pintool",
		"nt", "1", "Number of Threads per memory module");

KNOB<UINT32> knob_num_tlb(KNOB_MODE_WRITEONCE, "pintool",
		"tn", "32", "Number of Tlb entries");
static int ndp_tlb_entries;


//TODO: Implement as inputs
#define NDP_TLB_NUM_ENTRIES 16
#define MAX_NUM_THREADS 64

#define CL_SIZE 64


struct NDP_TLS_struct{
	TLB * tlb;		// 8 byte
	int8_t mem_id = 0;	// 1 byte
	uint32_t tlb_hits = 0;	// 4 byte
	uint32_t tlb_misses = 0;// 4 byte
	uint32_t pt_hits = 0;	// 4
	uint32_t pt_misses = 0;	// 4
	uint64_t _malloc_size = 0; // 8
	uint8_t CL_Pad[CL_SIZE - 33];	
};
typedef struct NDP_TLS_struct ndp_tls;



// Thread local storage 
static ndp_tls threads_data[MAX_NUM_THREADS];





NDP::PT * d_mem;


/*
 * Ignoring Size for now as we are only tracking
 * page accesses and assume that one access does 
 * not go across pages 
 * TODO: Think about page size
 */
static VOID SimulateMemOp
(
		VOID * addr, 
		UINT32 size, 
		THREADID thread_id 
)
{
	// For now the size of memOps is ignored as it is assumed that the whole 
	// memory access is on one page
	// TODO: Decide if this is a smart decision
	


	// Get Page
	if(thread_id == 0)
		return;

	int tid = thread_id - 1;
	ndp_tls * tls = &threads_data[tid]; 
	uint64_t page = ((uint64_t) addr) >> page_offset;
	int r = d_mem->acc_page(page, tls->mem_id);
	if(r !=  ACC_PAGE_NOT_HEAP)
	{
		if(tls->tlb->tlb_access(page) == TLB_HIT){
			tls->tlb_hits++;
		}
		else{
			tls->tlb_misses++;
		}
		if(r == ACC_PAGE_SUCC_LOCAL){
			tls->pt_hits++;
		}
		else{
			tls->pt_misses++;
		}
	}
}




static VOID ThreadStart(THREADID thread_id , CONTEXT *ctxt, INT32 flags, VOID * v)
{
	int tid = thread_id - 1;
	if(tid == -1)
		return;
	threads_data[tid].tlb  = new TLB(ndp_tlb_entries);
	// Calculate mem id
	int num_mems = knob_num_mems.Value();
	int tpm = knob_num_threads.Value();
	int8_t mem_id = (tid/tpm);
	if(mem_id >= num_mems){
		std::cerr << "Invalid values for num mems or threads per mem mod!\n";
	}
	threads_data[tid].mem_id = mem_id;
	//printf("Start: %d | %d \n", threads_data[tid].pt_hits,threads_data[tid].tlb_hits);	
}


static VOID ThreadFini(THREADID thread_id, const CONTEXT *ctxt, INT32 flags, VOID * v)
{
	int tid = thread_id - 1;

	ndp_tls * tls = &threads_data[tid]; 
	delete tls->tlb;
}

static VOID Instruction(INS ins, VOID * v)
{
	UINT32 memOperands = INS_MemoryOperandCount(ins);
	for(UINT32 memOp =0; memOp < memOperands; memOp++){
		UINT32 size = INS_MemoryOperandSize(ins,memOp);
		INS_InsertCall(ins, IPOINT_BEFORE, 
			(AFUNPTR) SimulateMemOp,
			IARG_MEMORYOP_EA, memOp,
			IARG_UINT32, size,
			IARG_THREAD_ID,
			IARG_END);
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
	uint64_t start = (addr) >> page_offset;
	uint64_t stop = (addr + size - 1) >> page_offset;
#ifdef DEBUG_INFO
	printf("MALLOC TID : %d \n" , tid);
	printf("addr: %lu ;size: %lu \n",addr,size);
	printf("start: %lu ;stop: %lu \n",start,stop);
#endif
	d_mem->add_memblock(start,stop);

}

/**
 * Checks wether the called routine is MALLOC or free
 * if so adds a memory block to it tracking the pages
 *
**/
static VOID IMAGE(IMG img, VOID *v)
{	
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
	

#ifdef DEBUG_NDP
	for( SEC sec= IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec) ){
		for( RTN rtn= SEC_RtnHead(sec); RTN_Valid(rtn); rtn = RTN_Next(rtn) ){
			std::cout << RTN_Name(rtn) << std::endl;
		}
	};
#endif

}

INT32 Usage(){
	cerr << "Tool to analyze NDP benchmarks \n ";
        cerr << KNOB_BASE::StringKnobSummary() << endl;
	return -1;
};

VOID Fini(INT32 code, VOID *v){
	PIN_RWMutexFini(&mem_block_lock);
	ndp_tls * tls;
	for(int i = 0; i < MAX_NUM_THREADS; i++){
		tls = &threads_data[i];
		if((tls->tlb_hits == 0) && (tls->tlb_misses == 0)){
			break;
		}
		cout << "TID: " << i << endl;
		cout << "TLB Hits: " << tls->tlb_hits << endl;
		cout << "TLB Misses: " << tls->tlb_misses << endl;
		cout << "pt Hits: " << tls->pt_hits << endl;
		cout << "pt Misses: " << tls->pt_misses << endl;

	}
	delete d_mem;
}

// Print Configuration for current Run
// TODO: Implement missing variables
int print_info(int nm){
	printf("Pintool for NDP \n");
	printf("----------------------------\n");
	printf("# Memory cubes 		: %d\n", nm);
	printf("# Threads/Memory Cube	: %d\n", knob_num_threads.Value());
	printf("Page size		: %d\n", ndp_page_size);
	printf("TLB entries		: %d\n", ndp_tlb_entries);
	//printf("TLB assoziativity	: %d:, ?);
	printf("----------------------------\n");
	return 0;
}

int main(int argc, char * argv[])
{

	PIN_InitSymbols();
	if(PIN_Init(argc,argv)){
		return Usage();
	}
	PIN_RWMutexInit(&mem_block_lock);
	ndp_page_size = knob_page_size.Value();
	ndp_tlb_entries = knob_num_tlb.Value();
	int num_mems = knob_num_mems.Value();
	print_info(num_mems);
	// Allocate new pagetrack
	d_mem = new NDP::PT(num_mems);
	page_offset = log_2_uint64_t(ndp_page_size);

	INS_AddInstrumentFunction(Instruction,0);
	IMG_AddInstrumentFunction(IMAGE,0);
	
	PIN_AddThreadStartFunction(ThreadStart, 0);
	PIN_AddThreadFiniFunction(ThreadFini,0);
	PIN_AddFiniFunction(Fini,0);

	// Never Return 
	PIN_StartProgram();
	return 0;
}


