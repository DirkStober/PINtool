#include <iostream>

#ifndef TLB_H
#define TLB_H 100

#define TLB_HIT 1
#define TLB_MISS 0




class TLB {
	public:
		TLB(int num_entries);
		~TLB();
		uint64_t * entries;
		int index_bits;
		int tlb_access(uint64_t page_addr);
};
#endif
