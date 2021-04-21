#include "pagetrack.h"
#include <cstring>

using namespace NDP;





PT::PT(uint64_t num_mem_in){
	num_mem = num_mem_in;
	high_addr = ~(0b0);
	low_addr = ~(0b0);

	lock_check = 0;
	
}

PT::~PT(){
	if((high_addr == !(0b0)) || ( low_addr == !(0b0))){
		free(page_entries);
	}
	return;
}

#if (MEM_DIST == FIRST_TOUCH)

int init_pages(int8_t * pe, int n){
	for(int i = 0; i < n ; i++){
		pe[i] = -1;
	}
	return 0;
}


int PT::add_memblock(uint64_t p_start, uint64_t p_stop){
	if(lock_check > 0){
		std::cerr << " Lock failure multiple threads in add memblock ! \n";
		return -1;
	}
	lock_check++;
	// if not at all initialized
	if((high_addr == ~((uint64_t) 0b0)) && (low_addr == ~((uint64_t) 0b0))){
		int new_size = p_stop - p_start +1;
		page_entries = (int8_t *) malloc(sizeof(int8_t)*(new_size));
		init_pages(page_entries, (new_size));
		high_addr = p_stop;
		low_addr = p_start;
		lock_check--;
		return 0;
	};


	// Do nothing if mem region is already initialized
	if((p_start >= low_addr)  && (p_stop <= high_addr)){
		lock_check--;
		return 0;
	}
	// New high and low addr
	uint64_t new_high = (p_stop > high_addr) ? p_stop : high_addr;
	uint64_t new_low = (p_start < low_addr) ? p_start : low_addr;
	// New page 
	uint64_t new_size = new_high - new_low + 1;
	int8_t * tmp = (int8_t *)  malloc(sizeof(int8_t) * new_size);
	// Mem copy old chunk to new array
	uint64_t l_i = low_addr - new_low;
	uint64_t r_i = new_high - high_addr;
	memcpy(&tmp[l_i],page_entries,sizeof(int8_t)*(high_addr - low_addr + 1));
	// Initialize upper and lower parts 
	init_pages(tmp, l_i );
	init_pages(&tmp[high_addr - new_low + 1], r_i);
	free(page_entries);
	page_entries = tmp;
	high_addr = new_high;
	low_addr = new_low;
	lock_check--;
	return 0;
}
	
int PT::acc_page(uint64_t p_addr, int8_t mem_id){
	// See if page on heap
	if((p_addr < low_addr) || (p_addr > high_addr)){
		return ACC_PAGE_NOT_HEAP;
	}
	// Get Page 
	int8_t * page = &page_entries[p_addr - low_addr];
	if(*page == mem_id){
		return ACC_PAGE_SUCC_LOCAL;
	}
	else if(*page > -1){
		return ACC_PAGE_SUCC_NOT_LOCAL;
	}
	else{
		*page = mem_id;
		return ACC_PAGE_SUCC_LOCAL;
	}
}	
#else
int PT::add_memblock(uint64_t p_start, uint64_t p_stop){
	if(lock_check > 0){
		std::cerr << " Lock failure multiple threads in add memblock ! \n";
		return -1;
	}
	lock_check++;
	high_addr = (p_stop > high_addr) ? p_stop : high_addr;
	low_addr = (page_start < low_addr) ? page_start : low_addr;
	lock_check--;
	return 0;
}
int PT::acc_page(uint64_t p_addr, int8_t mem_id){
	if((p_addr < low_addr) || (p_addr > high_addr)){
		return ACC_PAGE_NOT_HEAP;
	}
	int8_t tmp  = (int8_t) (p_addr % num_mem);
	if(tmp == mem_id){
		return ACC_PAGE_SUCC_LOCAL;
	}
	else{
		return ACC_PAGE_SUCC_NOT_LOCAL;
	}
}
#endif


