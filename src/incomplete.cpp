#include <iostream>
#include <vector>
#include <x86intrin.h>

#include "DenseMatrix.hpp"
#include "CSCMatrix.hpp"
#include "Symbolic.hpp"
#include "Incomplete.hpp"
#include "Name.hpp"
#include "CSCOperations.hpp"
#include "SPDTester.hpp"

using namespace std;
typedef uint64_t cpucycle;

int main(int argc, char* argv[])
{
  cpucycle begin = __rdtsc();
  double alpha = 0;
  if (argc > 1)
    alpha = stod(argv[1]); //read in alpha from cmd line if available

  string name = "name here";
  const int x = 100; //chose size of matrix (1-100 produced by convert.m)
  // matlab files are created by convert.m
  TripletMatrix tmat = readMatlabFile(matrixDir + to_string(x) + suffix);

  // Dense matrix support for smaller hand made matrices see DenseMatrix.hpp
//  DenseMatrix dmat = matrixLDLTest();

  // Compresses the matrix into CSC format
  CSCMatrix cmat = convertTripletToCSC(tmat);
//  CSCMatrix cmat = convertDenseToCSC(dmat);

  if (!isSPD(cmat)) //unfinished SPD check
    cout << "input matrix was not symmetric positive definite" << endl;

  CSCMatrix cmat3(cmat); //copy for later error checking
  
  // Symbolic Analysis
  vector<Set> scriptL = symbolicWithQuotient(cmat);

  // LDL Factorization
  cpucycle begin2 = __rdtsc();
  CSCMatrix lmat;
  vector<double> dvec;
  tie(lmat, dvec) = incompleteLDL(scriptL, cmat, alpha);
  cpucycle end2 = __rdtsc();

  // output matrix to file
//  TripletMatrix outMat = convertCSCToTriplet(lmat);
//  writeMatlabFile(outMat, dvec, lFactorDir + name + suffix, dFactorDir + name + suffix);

  // Checking error
  CSCMatrix cmat2 = multiplyLDL(lmat, dvec);
  subtractMatrix(cmat3, cmat2);
  cout << "subtracting matrices: " << endl << cmat3 << endl;
  cout << "magnitude of original matrix: " << matrix2NormMag(cmat) << endl;
  cout << "magnitude of back-multiplied matrix: " << matrix2NormMag(cmat2) << endl;
  cout << "magnitude of subtracted matrix: " << matrix2NormMag(cmat3) << endl; 
  cout << "program complete at " << __rdtsc()-begin << " with factor taking " << end2-begin2 << endl;
  cout << getCounters() << endl;
  exit(0);
}
