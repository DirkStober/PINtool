#ifndef PAGETRACK_H
#define PAGETRACK_H 1
#include <cstdint>
#include <iostream>

// Return values for access_page
#define ACC_PAGE_NOT_HEAP 	(0b00)
#define ACC_PAGE_SUCC_LOCAL 	1
#define ACC_PAGE_SUCC_NOT_LOCAL	(0b10) 


#define FIRST_TOUCH 0
#define MEM_DIST FIRST_TOUCH


namespace NDP{
class PT {
	public:
	PT(uint64_t num_mem_in);
	~PT();
	int add_memblock(uint64_t p_start, uint64_t p_stop);  
	uint32_t acc_page(uint64_t page, int8_t mem_id);
	// If mem access is above this it is assumed to access the stack
	uint64_t high_addr;
	uint64_t low_addr;
	uint64_t num_mem;
	// Store page end addresses in one array
	// and start pages in other
	int8_t *  page_entries;
	// Control bit
	int lock_check;


};
}
#endif
