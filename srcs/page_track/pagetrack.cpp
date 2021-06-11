#include "pagetrack.h"
#include <cstring>

using namespace NDP;


static uint64_t log_2_uint64_t(uint64_t a){
	uint64_t result = 0;
	while(a >>= 1){
		result++;
	}
	return result;
}


/*
 * Simple function to set up all new pages to -1 
 */
inline int init_pages(int8_t * pe, int n){
	for(int i = 0; i < n ; i++){
		pe[i] = -1;
	}
	return 0;
}

PT_FT::PT_FT(int8_t num_mem_in){
	num_mem = num_mem_in;
	high_addr = ~(0b0);
	low_addr = ~(0b0);

	
}



PT_FT::~PT_FT(){
	if((high_addr == !(0b0)) || ( low_addr == !(0b0))){
		free(page_entries);
	}
	return;
}

int PT_FT::add_memblock(uint64_t p_start, uint64_t p_stop){
	// if not at all initialized
	if((high_addr == ~((uint64_t) 0b0)) && (low_addr == ~((uint64_t) 0b0))){
		int new_size = p_stop - p_start +1;
		page_entries = (int8_t *) malloc(sizeof(int8_t)*(new_size));
		init_pages(page_entries, (new_size));
		high_addr = p_stop;
		low_addr = p_start;
		return 0;
	};


	// Do nothing if mem region is already initialized
	if((p_start >= low_addr)  && (p_stop <= high_addr)){
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
	// TODO: This might be a problem!
	// Race condition with threads accessing page pointer could be dereferenced while other threads are reading values
	// Solution for now: Only a host thread is allowed to add memory blocks and must not allocate memory while other
	// threads are being analyzed
	tmp = __atomic_exchange_n(&page_entries,tmp,__ATOMIC_SEQ_CST);
	free(tmp);
	high_addr = new_high;
	low_addr = new_low;
	return 0;
}
	
uint32_t PT_FT::acc_page(uint64_t p_addr, int8_t mem_id){
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
		int8_t ret = __atomic_exchange_n(page, mem_id, __ATOMIC_RELAXED);
		if(ret == -1){
			return ACC_PAGE_SUCC_LOCAL;
		}
		else{
			return ACC_PAGE_SUCC_NOT_LOCAL;
		}
	}
}	



/*
 * Deconstructor
 */
PT_S::~PT_S(){}


/*
 * Constructor for static distribution of pages
 */
PT_S::PT_S(int8_t num_mem_in, uint64_t page_size, int blocks_per_page)
{
	num_mem = num_mem_in;
	// Get the offset for the addresses
	if(page_size % blocks_per_page){
		fprintf(stderr,"Page size %lu, not dividable by blocks per page %d !\n",
			       page_size,blocks_per_page);
		fprintf(stderr,"Block size set to %lu bytes\n",page_size);
		block_offset = log_2_uint64_t(page_size);
	}	
	else if((page_size / blocks_per_page ) < 64){
		fprintf(stderr,"Block size %d too small (min 64 bytes)\n",blocks_per_page);
		fprintf(stderr,"Block size set to 64 bytes\n");
		block_offset = log_2_uint64_t(64);
	}
	else{
		uint64_t tmp = page_size/blocks_per_page;
		block_offset = log_2_uint64_t(tmp);
	}
	// Set High and low addr to max uint64
	high_addr = ~(0b0);
	low_addr = ~(0b0);
}


/*
 * Add memory region to tracked heap, uses real addresses not pages !
 */
int PT_S::add_memblock(uint64_t a_start, uint64_t a_stop){
	if((high_addr == ~((uint64_t) 0b0)) && (low_addr == ~((uint64_t) 0b0))){
		high_addr = a_stop;
		low_addr = a_start;
	}
	else{
		if(a_stop > high_addr)
			high_addr = a_stop;
		if(a_start < low_addr)
			low_addr = a_start;
	}
	return 0;
}

/*
 * Access address and check locality 
 */
uint32_t PT_S::acc_addr(uint64_t addr, int8_t mem_id){
	if((addr < low_addr) || (addr > high_addr)){
		return ACC_PAGE_NOT_HEAP;
	}
	int8_t res = (addr >> block_offset) % num_mem;
	if(res == mem_id){
		return ACC_PAGE_SUCC_LOCAL;
	}
	return ACC_PAGE_SUCC_NOT_LOCAL;
}



