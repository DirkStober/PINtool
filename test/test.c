#include <stdio.h>    
#include <stdlib.h>   
#include <time.h>





int main(int argc, char * argv[])
{
	float * a;
	float * b;
	float * c;
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
	for(i = 0; i < 500; i++){
		c[i] = a[i] * b[i];
		c[2*i] = a[i] / b[i];
	}
	free(a);
	free(b);
	free(c);


	return 0;

}

