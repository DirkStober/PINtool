
//@HEADER
// ***************************************************
//
// HPCG: High Performance Conjugate Gradient Benchmark
//
// Contact:
// Michael A. Heroux ( maherou@sandia.gov)
// Jack Dongarra     (dongarra@eecs.utk.edu)
// Piotr Luszczek    (luszczek@eecs.utk.edu)
//
// ***************************************************
//@HEADER

/*!
 @file ComputeWAXPBY.cpp

 HPCG routine
 */

#include "ComputeWAXPBY.hpp"
#include "omp.h"

#include <cstdio>
int __attribute__((noinline)) test123(const Vector &y,Vector &w,const Vector &x){
	double * wv = w.values;
	for(int i = 0; i < 4; i++){
		wv[i] = i;
	}


	return 0;
}


int __attribute__((noinline)) ComputeWAXPBY(const local_int_t n, const double alpha, const Vector & x,
    const double beta, const Vector & y, Vector & w, bool & isOptimized) {

  // This line and the next two lines should be removed and your version of ComputeWAXPBY should be used.
  isOptimized = false;
  //test123(y,w,x);
  assert(x.localLength>=n); // Test vector lengths
  assert(y.localLength>=n);

  const double * const xv = x.values;
  const double * const yv = y.values;
  double * const wv = w.values;
  

  #pragma omp parallel
  {
#pragma omp for
  	for (local_int_t i=0; i<n; i++){
		wv[i] = alpha * xv[i] + beta * yv[i];
  	}
  }
  return 0;
}
