#include <iostream>

#ifndef TLB_H
#define TLB_H 100

#define TLB_HIT (0b01)
#define TLB_MISS (0b10)




class TLB {
	public:
		TLB(int num_entries);
		~TLB();
		uint64_t * entries;
		int index_bits;
		uint32_t tlb_access(uint64_t page_addr);
};
#endif
