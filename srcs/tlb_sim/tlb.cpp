#include "tlb.h"

	



uint64_t log_2_int(uint64_t a){
	uint64_t result = 0;
	while(a >>= 1){
		result++;
	}
	return result;
}

TLB::TLB(int num_entries, int p_size){
       entries = new uint64_t[num_entries]() ;
       hits =0;
       page_size = p_size;
       misses = 0;
       page_offset = log_2_int(page_size);
       index_bits = num_entries - 1;

       // initialize the first cell to a value != 0 
       entries[0] = 0xFF;
}

TLB::~TLB(){
	delete entries;
}

int TLB::tlb_access(uint64_t virtual_addr){
	// Shift the address by the size of the page
	uint64_t used_addr = (virtual_addr >> page_offset); 
	// Get index 
	uint64_t index = used_addr & (index_bits);
	// check if address is used?
	int hit=0;
	//std::cout << "addr: "<< std::hex << used_addr << std::endl;
	//std::cout << "Ib: " << index_bits << std::endl;
	//std::cout << "Index: " << index << std::endl;
	if(entries[index] == used_addr){
		hits++;
		hit = 1;
	}
	else{
		entries[index] = used_addr;
		misses++;
		hit = 0;
	}
	return hit;
}





