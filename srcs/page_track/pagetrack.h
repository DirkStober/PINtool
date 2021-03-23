#ifndef PAGETRACK_H
#define PAGETRACK_H 1
#include <vector>
#include <cstdint>
#include <iostream>

// Return values for access_page
#define ACC_PAGE_SUCC 0
#define ACC_PAGE_ABOVE 1
#define ACC_PAGE_NOT_FOUND 2



// TODO: Figure out a more effective way?
inline int atomic_exchange(int value, int * target){
	//load value into register
	int result;
	asm (
			"xchgl %1 , %2;"
			"movl %1 , %0;"
			: "=&r" (result)
			: "r" (value)  ,  "m" (*target)
			:  "memory" 
			);
	return result;
}


namespace NDP{

struct mem_entry{
	// Points to the top most page
	int * top_page;
	// Internal page completly filled by this allocation
	int * int_pages; 
	// Points to the bottom most page
	int * bot_page; 
};



class PT {
	public:
	PT(int p_size, int num_t);
	~PT();
	int add_memblock(uint64_t mem_start, uint64_t mem_size);  
	int acc_page(uint64_t addr, int new_value, int * prev_value);
	int rem_memblock(uint64_t mem_start, uint64_t mem_size);
	int page_size;
	int page_off;
	int num_threads;
	// If mem access is above this it is assumed to access the stack
	uint64_t high_addr;

	// Store page end addresses in one array
	// and start pages in other
	// TODO: Use std::vector ?
	std::vector<uint64_t>  stop_page_addr;
	std::vector<uint64_t>  start_page_addr;
	std::vector<uint64_t>  start_addr;
	std::vector<uint64_t>  stop_addr;
	std::vector<struct mem_entry>  mem_entries;



	
	
};



}
#endif
