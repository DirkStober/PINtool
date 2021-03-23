#include <iostream>
#include <cstdio>
#include "pin.H"
#include "tlb.h"
#include "pagetrack.h"

#define MALLOC "malloc"
#define FREE "free"


#define MEM_READ 1
#define MEM_WRITE 2
#define MEM_READWRITE 3

#define MAX_NUM_THREADS 64



#define NDP_PAGE_SIZE 2048

struct tlb_results{
	uint64_t hits;
	uint64_t misses;
};

PIN_LOCK pinLock;
TLB * tlbs[MAX_NUM_THREADS];
NDP::PT * d_mem;
struct tlb_results tlbs_results[MAX_NUM_THREADS];
int counter;

VOID RecordMemOp(VOID * ip, VOID * addr , UINT32 acs_t , UINT32 size, 
		VOID * tid)
{
	int p_v;
	tlbs[(uint64_t) tid]->tlb_access((uint64_t) addr);
	if(d_mem->acc_page(((uint64_t) addr), 0 , &p_v ) == ACC_PAGE_SUCC){
		counter++;
	}
}

INT32 Usage(){
	std::cerr << "This is a test file";
	return -1;
};

VOID Fini(INT32 code, VOID *v){
	printf("Results: %d \n", counter);
	for(int i = 0; i < MAX_NUM_THREADS; i++){
		if((tlbs_results[i].hits == 0) && (tlbs_results[i].misses == 0)){
			break;
		}
		printf("\nTID: %d \n H: %lu | M: %lu \n",i,tlbs_results[i].hits,
		tlbs_results[i].misses);
	}
	delete d_mem;
}



VOID ThreadStart(THREADID threadid, CONTEXT *ctxt, INT32 flags, VOID * v)
{
	PIN_GetLock(&pinLock,threadid+1);
	printf("thread starting! tid: %d\n",threadid);
	PIN_ReleaseLock(&pinLock);
	tlbs[threadid] = new TLB(16,2048);
}


VOID ThreadFini(THREADID threadid, const CONTEXT *ctxt, INT32 flags, VOID * v)
{
	PIN_GetLock(&pinLock,threadid+1);
	printf("thread Finished! tid: %d\n",threadid);
	PIN_ReleaseLock(&pinLock);
	tlbs_results[threadid].hits = tlbs[threadid]->hits;
	tlbs_results[threadid].misses= tlbs[threadid]->misses;
	delete tlbs[threadid];
	delete d_mem;

}

VOID Instruction(INS ins, VOID * v)
{
	UINT32 memOperands = INS_MemoryOperandCount(ins);

	for(UINT32 memOp =0; memOp < memOperands; memOp++){
		UINT32 size = INS_MemoryOperandSize(ins,memOp);
		UINT32 acs_t = 0;
		if(INS_MemoryOperandIsWritten(ins,memOp)){
			acs_t += MEM_WRITE;
		}
		if(INS_MemoryOperandIsRead(ins,memOp)){
			acs_t += MEM_READ;
		}	
		INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR) RecordMemOp,
			     IARG_INST_PTR, 
			     IARG_MEMORYOP_EA, memOp,
			     IARG_UINT32, acs_t,
			     IARG_UINT32, size,
			     IARG_THREAD_ID,
			     IARG_END);
	}
}


VOID MallocBefore(uint64_t * s_out, ADDRINT size)
{
	*s_out = (uint64_t) size;
}

VOID MallocAfter(uint64_t * size, ADDRINT ret)
{
	// add mem block !!
	d_mem->add_memblock(ret,*size);
	std::cout << "  ALLOC " << std::hex << ret << " SIZE " << std::dec << *size << std::endl;

}

VOID Free(CHAR * name, ADDRINT ret)
{
	d_mem->rem_memblock(ret);
    	//std::cout << name << "(" << size << ")" << std::endl;
}

VOID IMAGE(IMG img, VOID *v)
{	
	uint64_t * size = new uint64_t;
	RTN mallocRtn = RTN_FindByName(img, MALLOC);
	if (RTN_Valid(mallocRtn))
	{
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
	delete size;
}



int main(int argc, char * argv[])
{

	PIN_InitSymbols();
	if(PIN_Init(argc,argv)){
		return Usage();
	}
	// Allocate new pagetrack
	d_mem = new NDP::PT(NDP_PAGE_SIZE);
	counter =0;

	INS_AddInstrumentFunction(Instruction,0);
	IMG_AddInstrumentFunction(IMAGE,0);
	
	PIN_AddThreadStartFunction(ThreadStart, 0);
	PIN_AddThreadFiniFunction(ThreadFini,0);
	PIN_AddFiniFunction(Fini,0);

	// Never Return 
	PIN_StartProgram();
	return 0;
}


