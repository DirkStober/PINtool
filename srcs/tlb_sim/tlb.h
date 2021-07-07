#include <iostream>

#ifndef TLB_H
#define TLB_H 100

#define TLB_HIT (0b01)
#define TLB_MISS (0b10)



class TLB {
	public:
		TLB(int num_entries_in, int asso_in);
		~TLB();
		int num_entries;
		int num_banks;
		int asso;
		uint64_t * entries;
		uint64_t * l_acc;
		uint64_t  counter;
		int index_bits;
		uint32_t tlb_access(uint64_t page_addr);
};
#endif
