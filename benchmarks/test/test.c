#include "omp.h"
#include "stdio.h"
#include <sys/mman.h>

int do_work(int * a){
#pragma omp parallel 
	{
	int tid = omp_get_thread_num();
	int i = tid * 1024;
	printf("I: %d \n",i);
	a[i] = i;
	}
	return 0;
}


void *mall(size_t size){
	void * a = mmap(0,size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON,
		-1, 0);
	return a;
};

int main(int argc, char * argv[])
{
	omp_set_num_threads(4);
	int n = 4096;
	int * a = (int * ) mall(4 *n ); 
	/*0
	int * a = (int*) mmap(0,4*n,
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON,
		-1, 0);
	*/
	printf("a ptr : %lu \n", a );
	do_work(a);
	munmap(a,n*4);

	return 0;
}
