#include "pagetrack.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "pthread.h"
#include "testCPP.h"
#include <shared_mutex>
std::shared_mutex mutex;





#define TEST_INTERACTIVE 1
// Test with User input 
int TestInteractive(){
	NDP::PT * test_pt = new NDP::PT(2048,1);	
	std::cout << "Hello this is the graphical test \n";
	int pg;
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
				std::cout << "New Val :";
				std::cin >> pg;
				r = test_pt->acc_page(b,pg,&pg);
				if(r == 0){
					std::cout << "Hit!\n";
					std::cout << "Prev Val: " << pg << std::endl;
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
	int  tmp = 0;
	int r = -10;
	testpt->acc_page(123,tmp,&r);
	
	delete testpt;
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
	int r;
	for(int i = 0; i < n; i++){
		if(tp->acc_page(acc[i],exp_wr[i],&r) != ret[i]){
			printf("Expected %d got %d\n",ret[i],r);
			return i+1;
		}
		if(exp_mem[i] != -1){
			if(exp_mem[i] != r){
				printf("Expected in page %d got %d\n",exp_mem[i],r);
				return i +1 ;
			}
		}
	}
	return 0;
}

int test1(){
	PT * tp = new PT(1024,1);
	tp->add_memblock(0,200);
	tp->add_memblock(201,400);
	tp->add_memblock(700,4096);
	uint64_t acc[] = {0,200,201,600,1024,1024,2048,2048,3091,7000};
	int ret[] = {0,2,0,0,0,0,0,0,0,1};
	int exp_mem[] = {-1,-1,5,3,0,9,0,6,0,-1};
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
	int tmp; 
	if(tp->acc_page(1293210,tmp,&tmp) != ACC_PAGE_SUCC){
		return 1;
	}
	tp->rem_memblock(0,t);
	int r = tp->acc_page(1293210,tmp,&tmp); 
	if(r != ACC_PAGE_ABOVE){
		fprintf(stderr,"Expected %d got %d \n",ACC_PAGE_ABOVE,r);
		return 1;
	}
	delete tp;

	return 0;
}


// test with multiple threads
// Main thread allocates and deallocates memory while others try to access it


struct testMT0_args{
	int tid;
	PT * page_t;
};

void * testMT0_threads(void * ptr);
int testMT0()
{
	int num_threads = 4;

	struct testMT0_args data_in[num_threads];

	PT * tp = new PT(2048,num_threads);

	pthread_t threads[num_threads];	
	for(int i = 1; i <num_threads; i++){
		data_in[i].page_t = tp;
		data_in[i].tid = i;
		pthread_create(&threads[i], NULL, testMT0_threads,&data_in[i]);
	};
	// main thread
	for(int i = 0; i < 20; i++){
		mutex.lock();
		tp->add_memblock(i*100,100);
		mutex.unlock();
	}
	for(int i = 0; i < 20; i++){
		mutex.lock();
		tp->rem_memblock(i*100,100);
		mutex.unlock();
	}
	for(int j=1; j < num_threads ; j++)
   	{
		pthread_join( threads[j], NULL);
   	}
	delete tp;
	return 0;
}

void * testMT0_threads(void * ptr)
{

	
	struct testMT0_args  * data_in = (struct testMT0_args *) ptr;
	int tid = data_in->tid;
	PT * tp = data_in->page_t;
	int tmp;
	int cnt = 0;
	for(int i = 0; i < 1000; i++){
		mutex.lock_shared();
		if(tp->acc_page(i*tid,tmp,&tmp) == ACC_PAGE_SUCC){
			cnt++;
		}
		mutex.unlock();
	}
	printf("Thread: %d, hits: %d \n",tid,cnt);
	return 0;
}





/* Put Test Cases Here */
TestFnPtr TestCases[] = {
	&testCD,
	&test1,
	&test2,
	&testMT0,
	NULL
};



int main(int argc, char * argv[]){
	int r =  testCPP(argc,argv,TestCases,&TestInteractive);
	return r;
}
