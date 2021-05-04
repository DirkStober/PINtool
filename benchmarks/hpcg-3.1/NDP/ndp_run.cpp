

//@HEADER
// ***************************************************
// Most of the code is copied from the main file in 
// the source folder ! 
// HPCG: High Performance Conjugate Gradient Benchmark
//
//
// ***************************************************
//@HEADER

/*!
 @file main.cpp

 HPCG routine
 */



#include <fstream>
#include <iostream>
#include <cstdlib>
using std::endl;

#include <vector>

#include "hpcg.hpp"

#include "CheckAspectRatio.hpp"
#include "GenerateGeometry.hpp"
#include "GenerateProblem.hpp"
#include "GenerateCoarseProblem.hpp"
#include "SetupHalo.hpp"
#include "CheckProblem.hpp"
#include "ExchangeHalo.hpp"
#include "OptimizeProblem.hpp"
#include "WriteProblem.hpp"
#include "ReportResults.hpp"
#include "mytimer.hpp"
#include "ComputeResidual.hpp"
#include "CG.hpp"
#include "CG_ref.hpp"
#include "Geometry.hpp"
#include "SparseMatrix.hpp"
#include "Vector.hpp"
#include "CGData.hpp"
#include "TestCG.hpp"
#include "TestSymmetry.hpp"
#include "TestNorms.hpp"

int main(int argc, char * argv[]) {


  HPCG_Params params;

  HPCG_Init(&argc, &argv, params);


  int size = params.comm_size, rank = params.comm_rank; // Number of MPI processes, My process ID


  local_int_t nx,ny,nz;
  nx = (local_int_t)params.nx;
  ny = (local_int_t)params.ny;
  nz = (local_int_t)params.nz;
  int ierr = 0;  // Used to check return codes on function calls

  ierr = CheckAspectRatio(0.125, nx, ny, nz, "local problem", rank==0);
  if (ierr)
    return ierr;

  /////////////////////////
  // Problem setup Phase //
  /////////////////////////


  // Construct the geometry and linear system
  Geometry * geom = new Geometry;
  GenerateGeometry(size, rank, params.numThreads, params.pz, params.zl, params.zu, nx, ny, nz, params.npx, params.npy, params.npz, geom);

  ierr = CheckAspectRatio(0.125, geom->npx, geom->npy, geom->npz, "process grid", rank==0);
  if (ierr)
    return ierr;

  // Use this array for collecting timing information
  std::vector< double > times(10,0.0);

  double setup_time = mytimer();

  SparseMatrix A;
  InitializeSparseMatrix(A, geom);

  Vector b, x, xexact;
  GenerateProblem(A, &b, &x, &xexact);
  SetupHalo(A);
  int numberOfMgLevels = 4; // Number of levels including first
  SparseMatrix * curLevelMatrix = &A;
  for (int level = 1; level< numberOfMgLevels; ++level) {
    GenerateCoarseProblem(*curLevelMatrix);
    curLevelMatrix = curLevelMatrix->Ac; // Make the just-constructed coarse grid the next level
  }

  setup_time = mytimer() - setup_time; // Capture total time of setup
  times[9] = setup_time; // Save it for reporting

  curLevelMatrix = &A;
  Vector * curb = &b;
  Vector * curx = &x;
  Vector * curxexact = &xexact;
  for (int level = 0; level< numberOfMgLevels; ++level) {
     CheckProblem(*curLevelMatrix, curb, curx, curxexact);
     curLevelMatrix = curLevelMatrix->Ac; // Make the nextcoarse grid the next level
     curb = 0; // No vectors after the top level
     curx = 0;
     curxexact = 0;
  }


  CGData data;
  InitializeSparseCGData(A, data);

  ///////////////////////////////
  // Optimized CG Timing Phase //
  ///////////////////////////////

  // Here we finally run the benchmark phase
  // The variable total_runtime is the target benchmark execution time in seconds



  /* This is the timed run for a specified amount of time. */

  double optTolerance = 0.0;  // Force optMaxIters iterations
  int optMaxIters = 40; // Choosen arbitrary
  int niters = 0;
  double normr0 = 0.0;
  double normr = 0.0;
  TestNormsData testnorms_data;
  testnorms_data.samples = 1;
  testnorms_data.values = new double[1];
  printf("Started NDP CG run...\n");
  printf("nx: %d ; ny: %d ; nz: %d \n", nx,ny,nz);
  printf("#threads: %d\n", params.numThreads);

  ZeroVector(x); // Zero out x
  ierr = CG( A, data, b, x, optMaxIters, optTolerance, niters, normr, normr0, &times[0], false);
  if (ierr) std::cout << "Error in call to CG: " << ierr << ".\n" << endl;
  if (rank==0) std::cout << "Scaled Residual [" << normr/normr0 << "]" << endl;
  testnorms_data.values[0] = normr/normr0; // Record scaled residual from this run

  // Compute difference between known exact solution and computed solution
  // All processors are needed here.

  // Test Norm Results
  ierr = TestNorms(testnorms_data);
  if(!testnorms_data.pass)
	  printf("Norms did not pass\n");

  ////////////////////
  // Report Results //
  ////////////////////
  printf("Result Timings \n");
  printf("Dot Product 	[ms]: %lf \n", times[1]);
  printf("WAXPY		[ms]: %lf \n", times[2]);
  printf("SPMV		[ms]: %lf \n", times[3]);
  printf("Precondition	[ms]: %lf \n", times[5]);
  printf("-------------------------------\n");
  printf("Total		[ms]: %lf \n", times[0]);
  

  // Clean up
  DeleteMatrix(A); // This delete will recursively delete all coarse grid data
  DeleteCGData(data);
  DeleteVector(x);
  DeleteVector(b);
  DeleteVector(xexact);
  delete [] testnorms_data.values;



  HPCG_Finalize();
  return 0;
}
