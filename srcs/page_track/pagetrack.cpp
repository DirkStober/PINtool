#include "pagetrack.h"
#include <cstring>
#include <unistd.h>

using namespace NDP;


// Returns the distance between both addresses if low_addr < high_addr
// else returns 0
uint64_t p_dist(uint64_t low_addr, uint64_t high_addr){
	if(low_addr >= high_addr){
		return 0;
	}
	else{
		return high_addr - low_addr;
	}
}
/*
 * Decides wheter the allocated memory is in the heap block  or
 * the mmap block
 */
mem_reg * heap_or_mmap(mem_reg * heap, mem_reg * mmap, uint64_t a_start, uint64_t page_offset){
	// Get program break to determine if allocation is on the heap or somewhere else !
	uint64_t b = (uint64_t)  sbrk(0);
	b = (b >> page_offset ) + 1;
	if(a_start >  b)
		return mmap;
	else
		return heap;
}

/*
 * Checks if address is in memory region
 */
bool in_mem(mem_reg * mem, uint64_t addr){
	if((addr < mem->low_addr) || (addr > mem->high_addr)){
		return false;
	}
	return true; 
}

/*
 * Just calculates the log_2 of ul
 */
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
inline int init_pages(int8_t * pe, uint64_t n){
	for(uint64_t i = 0; i < n ; i++){
		pe[i] = -1;
	}
	return 0;
}

PT_FT::PT_FT(int8_t num_mem_in){
	num_mem = num_mem_in;
	// Set High and low addr to max uint64
	heap.high_addr = (0b0);
	heap.low_addr = ~(0b0);
	mmap.high_addr = (0b0);
	mmap.low_addr = ~(0b0);
}



PT_FT::~PT_FT(){
	if(heap.high_addr != 0){
		free(heap.page_entries);
	}
	if(mmap.high_addr != 0){
		free(mmap.page_entries);
	}
	return;
}
int PT_FT::add_memblock(uint64_t p_start, uint64_t p_stop, uint64_t page_offset){
	mem_reg * mem = heap_or_mmap(&heap,&mmap,p_start,page_offset);
	// if not at all initialized
	if(mem->high_addr == 0){
		uint64_t new_size = p_stop - p_start +1;
		mem->page_entries = (int8_t *) malloc(sizeof(int8_t)*(new_size));
		init_pages(mem->page_entries, (new_size));
		mem->high_addr = p_stop;
		mem->low_addr = p_start;
		return 0;
	};
	// Do nothing if mem region is already initialized
	if((p_start >= mem->low_addr)  && (p_stop <= mem->high_addr)){
		return 0;
	}
	// New high and low addr
	uint64_t new_high = (p_stop > mem->high_addr) ? p_stop : mem->high_addr;
	uint64_t new_low = (p_start < mem->low_addr) ? p_start : mem->low_addr;
	// New page 
	uint64_t new_size = new_high - new_low + 1;
	int8_t * tmp = (int8_t *)  malloc(sizeof(int8_t) * new_size);
	// Mem copy old chunk to new array
	uint64_t l_i = mem->low_addr - new_low;
	uint64_t r_i = new_high - mem->high_addr;
	memcpy(&tmp[l_i],mem->page_entries,sizeof(int8_t)*(mem->high_addr - mem->low_addr + 1));
	// Initialize upper and lower parts 
	init_pages(tmp, l_i );
	init_pages(&tmp[mem->high_addr - new_low + 1], r_i);
	// TODO: This might be a problem!
	// Race condition with threads accessing page pointer could be dereferenced while other threads are reading values
	// Solution for now: Only a host thread is allowed to add memory blocks and must not allocate memory while other
	// threads are being analyzed
	tmp = __atomic_exchange_n(&mem->page_entries,tmp,__ATOMIC_SEQ_CST);
	free(tmp);
	mem->high_addr = new_high;
	mem->low_addr = new_low;
	return 0;
}
	
uint32_t PT_FT::acc_page(uint64_t p_addr, int8_t mem_id){
	mem_reg * mem;
	if((in_mem(&mmap,p_addr))){
		mem = &mmap;
	}
	else if(in_mem(&heap,p_addr)){
		mem = &heap;
	}
	else{
		return ACC_PAGE_NOT_HEAP;
	}
	// Get Page 
	int8_t * page = &(mem->page_entries[p_addr - mem->low_addr]);
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
	heap.high_addr = (0b0);
	heap.low_addr = ~(0b0);
	mmap.high_addr = (0b0);
	mmap.low_addr = ~(0b0);
}

/*
 * Add memory region to tracked heap, uses real addresses not pages !
 */
int PT_S::add_memblock(uint64_t a_start, uint64_t a_stop){
	mem_reg * mem = heap_or_mmap(&heap,&mmap,a_start,0);
	mem->low_addr  = a_start < mem->low_addr ? a_start : mem->low_addr;
	mem->high_addr  = a_stop > mem->high_addr ? a_stop : mem->high_addr;
	return 0;
}


/*
 * Access address and check locality 
 */
uint32_t PT_S::acc_addr(uint64_t addr, int8_t mem_id){
	if((in_mem(&mmap,addr) | in_mem(&heap,addr) ) == false){
		return ACC_PAGE_NOT_HEAP;
	}
	int8_t res = (addr >> block_offset) % num_mem;
	if(res == mem_id){
		return ACC_PAGE_SUCC_LOCAL;
	}
	return ACC_PAGE_SUCC_NOT_LOCAL;
}



