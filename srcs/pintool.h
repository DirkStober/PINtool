#ifndef PINTOOL_H
#define PINTOOL_H 1
/*
 * Header file
 */

// Inlcude both tlb and pagetrack
#include "tlb.h"
#include "pagetrack.h"

#define CL_SIZE 64

struct NDP_TLS_struct{
	TLB * tlb;		// 8 byte
	int8_t mem_id = 0;	// 1 byte
	uint64_t tlb_hits = 0;	// 8 byte
	uint64_t tlb_misses = 0;// 8 byte
	uint64_t pt_hits = 0;	// 8
	uint64_t pt_misses = 0;	// 8
	uint64_t _malloc_size = 0; // 8
	uint8_t CL_Pad[CL_SIZE - 49];	
};
typedef struct NDP_TLS_struct ndp_tls;

struct NDP_parameters{
	int nm; // num memories
	int tpm; // threads per memory
	int page_size; // page size
	int tlb_entries; // tlb entries
	int tlb_asso; // tlb asso
	int page_distro; // page distro 
	int blocks_per_page; // for round robin distro
	ndp_tls * threads_data; // thread data structures
};
typedef struct NDP_parameters ndp_params;



int print_info(ndp_params * data_in);
int print_output(ndp_params * data_in);
int write_file(ndp_params * data_in, char ** input_args, const char * out_file);
uint64_t log_2_uint64_t(uint64_t a);
int get_footprint(NDP::PT_FT * pt, NDP_parameters * params);














#endif
