#ifndef testCPP_H
#define testCPP_H 1
#include <cstdio>
#include <cstdlib>





// Typedef pointer to test function:
typedef int TestFn();
typedef TestFn * TestFnPtr ;



int testCPP(int n, char * args[], TestFnPtr * TCPP_TestCases);



#endif