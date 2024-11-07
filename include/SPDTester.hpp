#pragma once

#include <vector>
#include "CSCMatrix.hpp"
#include <cmath>

using namespace std;


bool isSymmetric(const CSCMatrix& cmat) // also checks for positive diagonal elements
{
  int dim = cmat.p.size() - 1;
  for (int i = 0; i < dim; i++) {
    for (int j = cmat.p[i]; j < cmat.p[i+1]; j++) {
      if (cmat.i[j] < i) { // above diagonal, skip b/c redundant
        continue;
      }
      else if (cmat.i[j] == i) { // on the diagonal, check that it's positive for free
        if (cmat.a[j] < 0)
          return false;
      }
      else { // below diagonal, check that swapping x and y gives same value
	if (cmat.i[j] >= (int)cmat.p.size()) { // matrix square check
          // cout << "non-square matrix found" << endl;
          return false;
        }
        for (int k = cmat.p[cmat.i[j]]; k < cmat.p[cmat.i[j] + 1]; k++) {
          if (cmat.i[k] == i) {
            if (cmat.a[k] != cmat.a[j])
              return false;
          }
        }
      }
    }
  }
  return true;
}

bool passesVectorDiagRule(const CSCMatrix& cmat)
{
  int dim = cmat.p.size() - 1;
  for (int i = 0; i < dim; i++) {
    int counter = 0;
    for (int j = cmat.p[i]; j < cmat.p[i+1]; j++) {
      if (i == cmat.i[j])
        counter += abs(cmat.a[j]);
      else
        counter -= abs(cmat.a[j]);
    }
    if (counter <= 0)
      return false;
  }
  return true;
}

bool passesFullSPDTest() //const CSCMatrix& cmat)
{
  return true;
}

bool isSPD(const CSCMatrix& cmat)
{
  if (!isSymmetric(cmat)) {
    cout << "Not symmetric, "; 
    return false;
  }
  if (passesVectorDiagRule(cmat)) {
    cout << "Skips full check, ";
    return true;
  }
  return passesFullSPDTest(); //cmat);
}


