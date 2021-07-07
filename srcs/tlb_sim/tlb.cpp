#include "tlb.h"



TLB::TLB(int num_entries_in, int asso_in){
	num_entries = num_entries_in;
	asso = asso_in;
	entries = new uint64_t[num_entries]() ;
	l_acc = new uint64_t[num_entries]() ;
	counter = 0;
	num_banks = num_entries/asso;
       	index_bits = num_banks - 1;
       	// initialize the first cell to a value != 0 
	for(int i = 0 ; i < num_entries; i++){
		l_acc[i] = 0;
		entries[i] = ~((uint64_t) 0b0);
	}
}


TLB::~TLB(){
	delete entries;
	delete l_acc;
}

uint32_t TLB::tlb_access(uint64_t page_addr){
	counter++;
	// Get index 
	uint64_t index = page_addr & (index_bits);
	// check if address is used?
	for(int i = 0; i < asso; i++){
		if(entries[index*asso+i] == page_addr){
			l_acc[index*asso + i] = counter;
			return TLB_HIT;
		}
	}

	// Find entry that has to be replaced
	uint64_t target = ~(0b0);
	int lru_page = 0;
	for(int i = 0 ; i < asso ; i++){
		if(l_acc[index*asso + i] < target){
			target = l_acc[index*asso + i];
			lru_page = index*asso + i;
		}
	}
	l_acc[lru_page] = counter;
	entries[lru_page] = page_addr;
	return TLB_MISS;
}
