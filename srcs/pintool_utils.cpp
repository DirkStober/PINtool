/*
 * File containing utils to print outputs of the pintool
 * This is mainly used to clean up the main pintool file
 */
#include "pintool.h"
#include <fstream>



// Print Configuration for current Run
// TODO: Implement missing variables
int print_info(ndp_params * data_in){
	printf("Pintool for NDP \n");
	printf("----------------------------\n");
	printf("# Memory cubes 		: %d\n", data_in->nm);
	printf("# Threads/Memory Cube	: %d\n", data_in->tpm);
	printf("Page size		: %d\n", data_in->page_size);
	printf("TLB entries		: %d\n", data_in->tlb_entries);
	printf("TLB asso		: %d\n", data_in->tlb_asso);
	printf("----------------------------\n");
	return 0;
}

int print_output(ndp_params * data_in){
	int total_t = data_in->tpm * data_in->nm;
	ndp_tls * tls;
	for(int i = 0; i < total_t; i++){
		tls = &data_in->threads_data[i];
		printf("TID: %d\n",i);
		printf( "TLB Hits:  %lu \n" ,tls->tlb_hits) ;
		printf( "TLB Misses:  %lu \n", tls->tlb_misses) ;
		printf( "pt Hits:  %lu \n" ,tls->pt_hits );
		printf( "pt Misses:  %lu \n", tls->pt_misses );
	}
	return 0;
}

// Write file as csv instead
int write_file
(
	ndp_params * data_in,
	char ** input_args,
	const char * out_file
)
{
	FILE * of;
	of = fopen(out_file,"a");
	// Write General Data
	
	fprintf(of,"#mem cubes, threads/cubes, page size, #tlb entries, tlb asso, page distro, blocks/page , benchmark\n");
	fprintf(of,"%d, %d, %d, %d, %d, %d, %d,",data_in->nm,
			data_in->tpm,data_in->page_size,
			data_in->tlb_entries, data_in->tlb_asso, data_in->page_distro, data_in->blocks_per_page);
	int on =0;
	for(int i =0; input_args[i] != NULL; i++){
		if(on)
			fprintf(of,"%s ",input_args[i]);
		if(input_args[i][0] == '-' && input_args[i][1] == '-')
			on =1;
	}
	fprintf(of,"\n");
	int nt = data_in->nm * data_in->tpm;
	fprintf(of,"TID, TLB HITS, TLB MISSES, tlb hit ratio, PAGE LOCAL, PAGE NOT LOCAL, PAGE HIT RATIO\n");
	ndp_tls * tls;
	for(int i = 0; i < nt ; i++){
		tls = &data_in->threads_data[i];
		fprintf(of,"%3d, ",i);
		double hm_ratio;
		uint64_t total_ins = tls->tlb_hits + tls->tlb_misses;
		hm_ratio = (tls->tlb_hits * 1.0)/((double) total_ins);
		fprintf(of,"%lu, %lu, %lf, ",tls->tlb_hits,tls->tlb_misses,hm_ratio);
		hm_ratio = (tls->pt_hits * 1.0)/(total_ins);
		fprintf(of,"%lu, %lu, %lf",tls->pt_hits,tls->pt_misses,hm_ratio);
		fprintf(of,"\n");
	}

	fprintf(of,"\n\n\n");
	fclose(of);
	return 0;
};


uint64_t log_2_uint64_t(uint64_t a){
	uint64_t result = 0;
	while(a >>= 1){
		result++;
	}
	return result;
}

int get_footprint(NDP::PT_FT * pt, NDP_parameters * params){
	uint64_t num_pages = 0;
	uint64_t pt_len = pt->heap.high_addr - pt->heap.low_addr;
	for(uint64_t i = 0; i < pt_len; i++){
		if( pt->heap.page_entries[i] > -1){
			num_pages++;
		}
	}
	pt_len = pt->mmap.high_addr - pt->mmap.low_addr;
	for(uint64_t i = 0; i < pt_len; i++){
		if( pt->mmap.page_entries[i] > -1){
			num_pages++;
		}
	}
	uint64_t res = params->page_size * num_pages;
	double f_res = res / (1024.0 * 1024.0);
	printf("Used Memory footprint: %lu B.\n", res);
	printf("Used Memory footprint: %lf MB .\n", f_res);
	return res;
}

