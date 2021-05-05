#include <stdio.h>    
#include <stdlib.h>   
#include <time.h>
#include <omp.h>
 
static int num_threads;



#define NT 4


__attribute__ ((noinline)) int do_work(float *a){
#pragma omp parallel for
	for(int i = 0; i < 200; i++)
	{
		a[i] = 5*i;
	}
	return 0;
}


int main(int argc, char * argv[])
{
	num_threads = 2;
	if(argc > 1){
		num_threads = atoi(argv[1]);
	}
	omp_set_num_threads(num_threads);

	printf("#threads: %d\n" , num_threads);
	float * a = (float * ) malloc(500*sizeof(float));
	float * b = (float * ) malloc(500*sizeof(float));
	float * c = (float * ) malloc(1000*sizeof(float));

	srand(time(NULL));
#pragma omp parallel for
	for(int i = 0; i < 200; i++)
	{
		a[i] = 2*i;
	}
	

	float avg = 0;
	for(int i = 0 ; i < 500; i++)
		avg+= a[i];
	printf("First avg: %f \n" , avg/500);

#pragma omp parallel for
	for(int i = 0; i < 200; i++)
	{
		a[i] = 3*i;
	}
	
	for(int i = 0 ; i < 500; i++)
		avg+= a[i];
	printf("Second avg: %f \n" , avg/500);
	do_work(a);

	for(int i = 0 ; i < 500; i++)
		avg+= a[i];
	printf("Third avg: %f \n" , avg/500);

	printf("Done\n");
	free(a);
	free(b);
	free(c);


	return 0;
}

