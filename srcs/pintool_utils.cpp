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

int write_file(ndp_params * data_in, const char * out_file){
	FILE * of;
	of = fopen(out_file,"a");
	// Write General Data
	fprintf(of,"START\n");
	fprintf(of,"2 META\n");
	fprintf(of,"#mem #t/mem pagesize #TLBentries\n");
	fprintf(of,"%d %d %d %d \n",data_in->nm,data_in->tpm,
			data_in->page_size,data_in->tlb_entries);
	int nt = data_in->nm * data_in->tpm;
	fprintf(of,"%d DATA\n",nt + 1);
	fprintf(of,"TID TLB_HITS TLB_MISSES TLB_HIT_RATIO PAGE_LOCAL PAGE_NOT_LOCAL PAGE_HIT_RATIO\n");
	ndp_tls * tls;
	for(int i = 0; i < nt ; i++){
		tls = &data_in->threads_data[i];
		fprintf(of,"%3d ",i);
		double hm_ratio;
		uint64_t total_ins = tls->tlb_hits + tls->tlb_misses;
		hm_ratio = (tls->tlb_hits * 1.0)/((double) total_ins);
		fprintf(of,"%lu %lu %lf ",tls->tlb_hits,tls->tlb_misses,hm_ratio);
		hm_ratio = (tls->pt_hits * 1.0)/(total_ins);
		fprintf(of,"%lu %lu %lf",tls->pt_hits,tls->pt_misses,hm_ratio);
		fprintf(of,"\n");
	}

	fprintf(of,"END\n");
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
