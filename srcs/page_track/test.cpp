#include "pagetrack.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "pthread.h"
#include "testCPP.h"
#include <shared_mutex>
std::shared_mutex mutex;










// Put Test Cases in here

using namespace NDP;
/* Test Constructor & Deconstructor */
int testCD(){

	PT * testpt = new PT(2);
	int  tmp = 0;
	int r = -10;
	testpt->acc_page(123,1);
	
	delete testpt;
	return 0;
}

int aux_run(
		PT *  tp, 
		uint64_t * acc, 
		int * ret , 
		int8_t * mem_id, 
		int n
	   )
{	
	int r;
	for(int i = 0; i < n; i++){
		int r = tp->acc_page(acc[i],mem_id[i]) ;
		if(r != ret[i]){
			printf("Expected %d got %d\n",ret[i],r);
			return i+1;
		}
	}
	return 0;
}

int test1(){
	PT * tp = new PT(4);
	tp->add_memblock(20,199);
	uint64_t acc[] = {0,199,200,199,20,20};
	int ret[] = {0,1,0,1,1,2};
	int8_t  mem_id[] = {0,0,0,0,1,0};
	int r = aux_run(tp,acc,ret,mem_id,6);	
	if(r){
		return r;
	};
	delete tp;
	return 0;
}

int test2(){
	PT * tp = new PT(4);
	tp->add_memblock(20,199);
	tp->add_memblock(6,6);
	tp->add_memblock(190,210);
	tp->acc_page(11,3);
	tp->acc_page(6,2);
	tp->add_memblock(2,300);
	uint64_t acc[] = 	{0,199,200,301,20,20, 6, 11};
	int ret[] = 		{0,  1,  1,  0, 1, 2, 1,  2};
	int8_t  mem_id[] = {0,0,0,0,1,0,2,2};
	int r = aux_run(tp,acc,ret,mem_id,8);	
	if(r){
		return r;
	};
	delete tp;
	return 0;
}






/* Put Test Cases Here */
TestFnPtr TestCases[] = {
	&testCD,
	&test1,
	&test2,
	NULL
};



int main(int argc, char * argv[]){
	int r =  testCPP(argc,argv,TestCases,NULL);
	return r;
}
