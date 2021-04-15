#include <stdio.h>    
#include <stdlib.h>   
#include <time.h>
#include <omp.h>


volatile __attribute__((noinline)) void _NDP_PIN_START_STOP_(int i){
	if(i){
		printf("NDP Pintool starts tracking now !\n");
	}
	else{
		printf("NDP Pintool stops tacking now !\n");
	};
	return;
 };
 


int main(int argc, char * argv[])
{
	float * a;
	float * b;
	float * c;
	a = (float * ) malloc(500*sizeof(float));
	b = (float * ) malloc(500*sizeof(float));
	c = (float * ) malloc(1000*sizeof(float));
	int i;

	omp_set_num_threads(2);
	srand(time(NULL));
	for(i = 0; i < 500; i++)
	{
		a[i] = rand() % 100 -50; 
		b[i] = rand() % 100 -50; 
	}
	_NDP_PIN_START_STOP_(1);
	#pragma omp parallel 
	{
		printf("TID: %d \n",omp_get_thread_num());
		for(i = 0; i < 500; i++){
			c[i] = a[i] * b[i];
		}
	}
	volatile int stop = 0;
	_NDP_PIN_START_STOP_(stop);
	free(a);
	free(b);
	free(c);


	return 0;

}

