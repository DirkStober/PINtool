#ifndef NDP_DEF_H 
#define NDP_DEF_H 0

/*
 * This file is the configuration file 
 * for the pintool used to simulate
 * memory accesses of NDP
**/

/*
 * Define memory layout 
**/
#define NDP_DEF_THR_PER_MEM 4
#define NDP_DEF_NUM_MEM 1

/*
 * Simulation parameters 
**/

/* 
 * Page distribution policy
 * for bigger strides use:
 * #define PAGE_DIST_STRIDE_N N
*/

#define PAGE_DIST_FIRST_TOUCH 0
#define PAGE_DIST_STRIDE_1  1
#define PAGE_DIST_STRIDE_2  2

#define NDP_DEF_PAGE_DIST 0

/*
 * Page Size in Bytes
**/

#define NDP_DEF_PAGE_SIZE 4096

/*
 * TLB organization
 * Associativity and number of entries
*/

// TODO: Introduce different associativity
#define NDP_DEF_TLB_ASSO 0
#define NDP_DEF_TLB_ENTRIES 16



#endif 
