#include <iostream>
#include <cstdio>
#include "pin.H"
#include "tlb.h"
#include "math.h"



#define MEM_READ 1
#define MEM_WRITE 2
#define MEM_READWRITE 3

#define MAX_NUM_THREADS 16

struct tlb_results{
	uint64_t hits;
	uint64_t misses;
};

PIN_LOCK pinLock;
TLB * tlbs[MAX_NUM_THREADS];
struct tlb_results tlbs_results[MAX_NUM_THREADS];


VOID RecordMemOp(VOID * ip, VOID * addr , UINT32 acs_t , UINT32 size, 
		VOID * tid)
{
	tlbs[(uint64_t) tid]->tlb_access((uint64_t) ip);
}

INT32 Usage(){
	std::cerr << "This is a test file";
	return -1;
};

VOID Fini(INT32 code, VOID *v){
	printf("Results: ");
	for(int i = 0; i < MAX_NUM_THREADS; i++){
		if((tlbs_results[i].hits == 0) && (tlbs_results[i].misses == 0)){
			break;
		}
		printf("\nTID: %d \n H: %lu | M: %lu \n",i,tlbs_results[i].hits,
		tlbs_results[i].misses);
	}
}



VOID ThreadStart(THREADID threadid, CONTEXT *ctxt, INT32 flags, VOID * v)
{
	PIN_GetLock(&pinLock,threadid+1);
	printf("thread starting! tid: %d\n",threadid);
	PIN_ReleaseLock(&pinLock);
	tlbs[threadid] = new TLB(16,4096);
}


VOID ThreadFini(THREADID threadid, const CONTEXT *ctxt, INT32 flags, VOID * v)
{
	PIN_GetLock(&pinLock,threadid+1);
	printf("thread Finished! tid: %d\n",threadid);
	PIN_ReleaseLock(&pinLock);
	tlbs_results[threadid].hits = tlbs[threadid]->hits;
	tlbs_results[threadid].misses= tlbs[threadid]->misses;
	delete tlbs[threadid];

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


int main(int argc, char * argv[])
{
    PIN_InitLock(&pinLock);

	if(PIN_Init(argc,argv)){
		return Usage();
	}

	INS_AddInstrumentFunction(Instruction,0);
	
	PIN_AddThreadStartFunction(ThreadStart, 0);
	PIN_AddThreadFiniFunction(ThreadFini,0);
	PIN_AddFiniFunction(Fini,0);

	// Never Return 
	PIN_StartProgram();
	return 0;
}


