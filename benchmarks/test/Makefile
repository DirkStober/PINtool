

omp: test.c
	gcc -O0 -fopenmp test.c -o test 


test: test.c
	gcc -S -fverbose-asm -o test.s -O0 -lpthread test.c
	gcc -o test test.c -O0 -lpthread
	 
