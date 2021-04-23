#include <stdio.h>    
#include <stdlib.h>   
#include <time.h>
#include <pthread.h>
 
static int num_threads;

float * a;
float * b;
float * c;

void * do_work(void * args){

	int tid = *(int *) args;
	int i;
	for(i = tid ; i < 104; i+=num_threads){
			c[i] = a[i] + b[i];
	}
	return NULL;
}





int main(int argc, char * argv[])
{
	num_threads = 2;
	if(argc > 1){
		num_threads = atoi(argv[1]);
	}

	printf("#threads: %d\n" , num_threads);
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
	int * thread_args = (int *) malloc(sizeof(int) * num_threads);
	pthread_t * threads = (pthread_t * ) malloc(sizeof(pthread_t)  *  num_threads);
	for(int i = 0; i < num_threads ; i++){
		thread_args[i] = i;
		pthread_create(&threads[i],NULL,do_work, &thread_args[i]);
	}
	printf("HEY");
	//thread_args[0] = 0;
	//do_work(&thread_args[0]);
	for(int i = 0; i < num_threads ; i++){
		pthread_join(threads[i],NULL);
	}
	
	free(a);
	free(b);
	free(c);


	return 0;
}

