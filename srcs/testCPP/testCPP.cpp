#include "testCPP.h"



int testCPP(int n, char ** args, TestFnPtr * TCPP_TestCases)
{	
	int choice;
	// If no input is given run all tests
	if(n < 2){
		printf("Running all tests...\n");
		int i = 0;
		while(TCPP_TestCases[i] != NULL){
			int r = TCPP_TestCases[i]();	
			if(r){
				fprintf(stderr, "Test %d failed with error %d!\n", i,r);
				return 1;
			}
			else{
				printf("Test %d passed.\n",i);	
			}
			i++;
		}
		printf("All %d tests passed.\n",i);
		return 0;
	}
	int select = atoi(args[1]);
	if(TCPP_TestCases[select] != NULL){
		printf("Running Test: %d ...\n",select);
		int r = TCPP_TestCases[select]();
		if(r){
			fprintf(stderr, "Test %d failed with error %d!\n",select,r);
		}
		else{
			fprintf(stdout, "Test %d passed. \n",select);
		}
		return r;
	}
	else{
		fprintf(stderr, "Test %d not implemented!\n",select);
		return -1;
	}
	return 0;
}
