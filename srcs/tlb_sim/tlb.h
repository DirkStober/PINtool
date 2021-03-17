#include <iostream>

#ifndef TLB_H
#define TLB_H 100

#define ADDR_LENGTH 64
#define TLB_ASSO 1


extern int hits;
extern int misses;

uint64_t log_2_int(uint64_t a);

class TLB {
	public:
		TLB(int num_entries, int p_size );
		~TLB();
		uint64_t * entries;
		int size;
		int page_size;
		int hits;
		int misses;
		int page_offset;
		int index_bits;
		int tlb_access(uint64_t virtual_addr);
};
#endif
