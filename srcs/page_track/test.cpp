#include "pagetrack.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "pthread.h"
#include "testCPP.h"


#define TEST_INTERACTIVE 1
// Test with User input 
int TestInteractive(){
	NDP::PT * test_pt = new NDP::PT(2048,1);	
	std::cout << "Hello this is the graphical test \n";
	int *pg;
	std::cout << "0: exit; 1: Add memory (start,size); 2: Acc addr (addr,val)\n";
	int a,r;
	std::cin >> a;
	while(a){
		uint64_t b,c;
		switch(a){
			case 1:
				std::cout << "Start: ";
				std::cin >> b;
				std::cout << "Size: ";
				std::cin >> c;
				test_pt->add_memblock(b,c);
				std::cout << std::endl;
				break;
			case 2:
				std::cout << "Addr: ";
				std::cin >> b;
				r = test_pt->get_page(b,&pg,0);
				if(r == 0){
					std::cout << "Hit!";
					std::cout << "Val: " << *pg << std::endl;
					std::cout << "New Val :";
					std::cin >> *pg;
				}
				else{
					std::cout << "No hit: " << r << std::endl;
				}
				break;
			default: 
				std::cout << "Wrong Entry: " << a << std::endl;
				break;
		}
		std::cout << "0: exit; 1: Add memory (start,size); 2: Acc addr\n";
		std::cin >> a;
		
	}
	delete test_pt;
	return 0;
}





// Put Test Cases in here

using namespace NDP;
/* Test Constructor & Deconstructor */
int testCD(){

	PT * testpt = new PT(2048,1);
	int ** tmp = new (int *);
	testpt->get_page(123,tmp,0);
	
	delete testpt;
	delete tmp;
	return 0;
}

int aux_run(
		PT *  tp, 
		uint64_t * acc, 
		int *ret , 
		int * exp_mem , 
		int * exp_wr ,
		int n
	   )
{	
	int ** tmp = new (int * );
	for(int i = 0; i < n; i++){
		if(tp->get_page(acc[i],tmp,0) != ret[i]){
			printf("Expected %d got %d\n",ret[i],**tmp);
			return i+1;
		}
		if(exp_mem[i] != -1){
			if(exp_mem[i] != **tmp){
				printf("Expected in page %d got %d\n",exp_mem[i],**tmp);
				return i +1 ;
			}
		}
		if(exp_wr[i] != -1){
			**tmp = exp_wr[i];
		}
	}
	delete tmp;
	return 0;
}

int test1(){
	PT * tp = new PT(1024,1);
	tp->add_memblock(0,200);
	tp->add_memblock(201,400);
	tp->add_memblock(700,4096);
	uint64_t acc[] = {0,200,201,600,1024,1024,2048,2048,3091,7000};
	int ret[] = {0,2,0,0,0,0,0,0,0,1};
	int exp_mem[] = {0,-1,5,3,0,9,0,6,0,-1};
	int exp_wr[] = {5,-1,3,-1,9,-1,6,-1,-1,-1};
	int r = aux_run(tp,acc,ret,exp_mem,exp_wr,10);	
	if(r){
		return r;
	};
	tp->rem_memblock(0,200);
	tp->rem_memblock(201,400);
	tp->rem_memblock(700,4096);
	delete tp;
	return 0;
}


int test2(){
	PT * tp = new PT(1073741824,1);

	uint64_t t  = 1073741824;
	t = t*3;
	tp->add_memblock(0,t);
	int ** tmp = new (int *);
	if(tp->get_page(1293210,tmp,0) != ACC_PAGE_SUCC){
		return 1;
	}
	tp->rem_memblock(0,t);
	int r = tp->get_page(1293210,tmp,0); 
	if(r != ACC_PAGE_ABOVE){
		fprintf(stderr,"Expected %d got %d \n",ACC_PAGE_ABOVE,r);
		return 1;
	}
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
	int r =  testCPP(argc,argv,TestCases);
	return r;
}
