#include "omp.h"
#include "stdio.h"
#include "stdlib.h"
#include <sys/mman.h>
#include <unistd.h>

extern char end;
int big_global[70000001];

int do_work(int * a,int c){
#pragma omp parallel 
	{
	int tid = omp_get_thread_num();
	int i = tid  ;
	printf("I: %d ; $sp: %p ; $bg: %p \n",i, (void *) &i , (void *) &big_global[70000000] );
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
	printf("    uninitialized data (end)  %10p\n", sbrk(0));
	int * a = (int * ) mall(4 *n ); 
	int * b = (int * ) malloc(4*n);
	printf("    uninitialized data (end)  %10p\n", sbrk(0));
	printf("a ptr : %p \n",(void *) a );
	printf("b ptr : %p \n", (void * )b );
	do_work(a,5);
	munmap(a,n*4);
	free(b);

	return 0;
}
