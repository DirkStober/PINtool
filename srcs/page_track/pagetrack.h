#include <vector>
#include <cstdint>
#include <atomic>


// Return values for access_page
#define ACC_PAGE_SUCC 0
#define ACC_PAGE_ABOVE 1
#define ACC_PAGE_NOT_FOUND 2


namespace NDP{

struct mem_entry{
	int * top_page; // Points to the top most page
	int * int_pages; // Internal page completly filled by this allocation
	int * bot_page; // Points to the bottom most page
};



class PT {
	public:
	PT(int p_size, int num_threads);
	~PT();
	int add_memblock(uint64_t mem_start, uint64_t mem_size);  
	int get_page(uint64_t addr, int ** mem_block, int tid  );
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


	// Use atomics to disallow get page during allocation
	// and deallocation of memory
	// Allocate one lock for every thread
	std::atomic_bool * locks;
	
};



}
