#include <stdio.h>    
#include <stdlib.h>   
#include <time.h>
#include <pthread.h>
 
#define num_threads 2

float * a;
float * b;
float * c;

void * do_work(void * args){

	int tid = *(int *) args;
	int i;
	for(i = tid ; i < 100; i+=num_threads){
			c[i] = a[i] + b[i];
	}
	printf("hello tid: %d \n",tid);
	return NULL;
}





int main(int argc, char * argv[])
{
	a = (float * ) malloc(500*sizeof(float));
	b = (float * ) malloc(500*sizeof(float));
	c = (float * ) malloc(1000*sizeof(float));

	int i;
	srand(time(NULL));
	for(i = 0; i < 500; i++)
	{
		a[i] = rand() % 100 -50; 
		b[i] = rand() % 100 -50; 
	}
	int thread_args[num_threads];
	pthread_t threads[num_threads];
	for(int i = 1; i < num_threads ; i++){
		thread_args[i] = i;
		pthread_create(&threads[i],NULL,do_work, &thread_args[i]);
	}
	thread_args[0] = 0;
	do_work(&thread_args[0]);
	for(int i = 1; i < num_threads ; i++){
		pthread_join(threads[i],NULL);
	}
	
	free(a);
	free(b);
	free(c);


	return 0;
}

