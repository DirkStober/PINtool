#include "omp.h"
#include "stdio.h"
#include "stdlib.h"





int main(int argc, char * argv[])
{
	printf("Hello World!\n");
	omp_set_num_threads(4);
	int * a = (int *) malloc(sizeof(int) * 4096);
#pragma omp parallel 
	{
		int tid = omp_get_thread_num();
		int i = tid * 1024;
		printf("TID: %d \n",i);
		int b = a[i];
	}
	free(a);
	return 0;
}
