#ifndef PAGETRACK_H
#define PAGETRACK_H 1
#include <cstdint>
#include <iostream>

// Return values for access_page
#define ACC_PAGE_NOT_HEAP 	(0b00)
#define ACC_PAGE_SUCC_LOCAL 	(0b01)
#define ACC_PAGE_SUCC_NOT_LOCAL	(0b10) 


#define PT_BREAK 	0x700000000000
#define PT_DIST		0x080000000000

struct mem_reg{
	uint64_t high_addr;
	uint64_t low_addr;
	int8_t * page_entries;
};



namespace NDP{

class PT_FT {
	public:
	PT_FT(int8_t num_mem_in);
	~PT_FT();
	int add_memblock(uint64_t p_start, uint64_t p_stop, uint64_t page_offset);  
	uint32_t acc_page(uint64_t page, int8_t mem_id);
	// If mem access is above this it is assumed to access the stack
	int8_t num_mem;
	// Store page end addresses in one array
	// and start pages in other
	mem_reg heap;
	mem_reg mmap;
};

class PT_S {
	public:
	PT_S(int8_t num_mem_in, uint64_t page_size, int blocks_per_page);
	~PT_S();
	int add_memblock(uint64_t a_start, uint64_t a_stop);  
	uint32_t acc_addr(uint64_t addr, int8_t mem_id);
	// If mem access is above this it is assumed to access the stack
	mem_reg heap;
	mem_reg mmap;
	int8_t num_mem;
	uint64_t block_offset;
};

}
#endif
