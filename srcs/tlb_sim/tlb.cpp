#include "tlb.h"

	




TLB::TLB(int num_entries){
       entries = new uint64_t[num_entries]() ;
       index_bits = num_entries - 1;
       // initialize the first cell to a value != 0 
       entries[0] = 0xFF;
}

TLB::~TLB(){
	delete entries;
}

int TLB::tlb_access(uint64_t page_addr){
	// Shift the address by the size of the page
	// Get index 
	uint64_t index = page_addr & (index_bits);
	// check if address is used?
	if(entries[index] == page_addr){
		return TLB_HIT;
	}
	else{
		entries[index] = page_addr;
		return TLB_MISS;
	}
}





