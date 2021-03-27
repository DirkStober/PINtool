/*
 * Code for Tracking memory acceses of the Binaries
 * Used with PIN 3.18
 *
 * Author: Dirk Stober <dirk.stober@posteo.de>
 */



#include <iostream>
#include <fstream>
#include "pin.H"




// Inlcude both tlb and pagetrack
#include "tlb.h"
#include "pagetrack.h"

// Define Malloc and free
#define MALLOC "malloc"
#define FREE "free"



#define MAX_NUM_THREADS 64

// Use a lock for access to allocate mem pages
PIN_RWMUTEX mem_block_lock;



using std::cout;
using std::cerr;
using std::endl;

KNOB<std::string> knob_output(KNOB_MODE_WRITEONCE,"pintool",
		"o", "ndp.out", "specify log file name");

KNOB<UINT32> knob_page_size(KNOB_MODE_WRITEONCE,"pintool",
		"p", "2048", "Page Size (bytes)");
	
KNOB<UINT32> knob_page_distribution(KNOB_MODE_WRITEONCE, "pintool",
		"org", "0", "Initial Page distribution (0 = First Touch,org > 0 => Stride with org ) ");

static int ndp_page_size;

#define CL_SIZE 64


struct NDP_TLS_struct{
	TLB * tlb;		// 8 byte
	int mem_id = 0;		// 4 byte
	uint32_t tlb_hits;	// 4 byte
	uint32_t tlb_misses;	// 4 byte
	uint32_t pt_hits = 0;	// 4
	uint32_t pt_misses = 0;	// 4
	uint8_t CL_Pad[CL_SIZE - 28];	
};
typedef struct NDP_TLS_struct ndp_tls;



// Thread local storage 
// TODO: Reimplement using TLS or/and 
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
		THREADID tid
)
{
	ndp_tls * tls = &threads_data[tid]; 
	int r = d_mem->acc_page((uint64_t) addr, tls->mem_id);
	if(r == ACC_PAGE_SUCC_LOCAL)
	{
		tls->pt_hits++;
		tls->tlb->tlb_access((uint64_t) addr);
	}
	else if(r == ACC_PAGE_SUCC_NOT_LOCAL)
	{
		tls->tlb->tlb_access((uint64_t) addr);
		tls->pt_misses++;
	}
}




static VOID ThreadStart(THREADID tid , CONTEXT *ctxt, INT32 flags, VOID * v)
{
	threads_data[tid].tlb  = new TLB(16,ndp_page_size);
}


static VOID ThreadFini(THREADID tid, const CONTEXT *ctxt, INT32 flags, VOID * v)
{

	ndp_tls * tls = &threads_data[tid]; 
	tls->tlb_hits = tls->tlb->hits;
	tls->tlb_misses= tls->tlb->misses;
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


static VOID MallocBefore(uint64_t * s_out, ADDRINT size)
{
	*s_out = (uint64_t) size;
}

static VOID MallocAfter(uint64_t * size, ADDRINT ret)
{
	// add mem block !!
	PIN_RWMutexWriteLock(&mem_block_lock);
	d_mem->add_memblock(ret,*size);
	PIN_RWMutexUnlock(&mem_block_lock);

}

static VOID Free(CHAR * name, ADDRINT ret)
{
	PIN_RWMutexTryReadLock(&mem_block_lock);
	d_mem->rem_memblock(ret);
	PIN_RWMutexUnlock(&mem_block_lock);
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
		uint64_t * size = new uint64_t;
		RTN_Open(mallocRtn);
		
		// Instrument malloc() to print the input argument value and the return value.
		RTN_InsertCall(mallocRtn, IPOINT_BEFORE, (AFUNPTR)MallocBefore,
		               IARG_ADDRINT, size,
		               IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
		               IARG_END);
		RTN_InsertCall(mallocRtn, IPOINT_AFTER, (AFUNPTR)MallocAfter,
				IARG_ADDRINT, size,
		               	IARG_FUNCRET_EXITPOINT_VALUE, IARG_END);
		
		RTN_Close(mallocRtn);
		delete size;
	}
	
	// Find the free() function.
	RTN freeRtn = RTN_FindByName(img, FREE);
	if (RTN_Valid(freeRtn))
	{
		RTN_Open(freeRtn);
	    	// Instrument free() to print the input argument value.
	    	RTN_InsertCall(freeRtn, IPOINT_BEFORE, (AFUNPTR)Free,
	    	               IARG_ADDRINT, FREE,
	    	               IARG_FUNCARG_ENTRYPOINT_VALUE, 0,
	    	               IARG_END);
	    	RTN_Close(freeRtn);
	}
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


int main(int argc, char * argv[])
{

	PIN_InitSymbols();
	if(PIN_Init(argc,argv)){
		return Usage();
	}
	PIN_RWMutexInit(&mem_block_lock);
	ndp_page_size = knob_page_size.Value();
	int init_dd = knob_page_distribution.Value();
	// Allocate new pagetrack
	d_mem = new NDP::PT(ndp_page_size,init_dd);

	INS_AddInstrumentFunction(Instruction,0);
	IMG_AddInstrumentFunction(IMAGE,0);
	
	PIN_AddThreadStartFunction(ThreadStart, 0);
	PIN_AddThreadFiniFunction(ThreadFini,0);
	PIN_AddFiniFunction(Fini,0);

	// Never Return 
	PIN_StartProgram();
	return 0;
}


