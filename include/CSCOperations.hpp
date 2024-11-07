#pragma once

#include <vector>
#include "CSCMatrix.hpp"
#include <cmath>

using namespace std;

CSCMatrix buildLowerTriangle(const CSCMatrix& lmat, const vector<double>& dvec)
{
  CSCMatrix result;
  int dim = lmat.p.size() - 1;
  for (int i = 0; i < dim; i++) 
    result.p.push_back(0);
  for (int i = 0; i < dim; i++) {
    for (int j = lmat.p[i]; j < lmat.p[i+1]; j++) {
      for (int k = j; k < lmat.p[i+1]; k++) {
        bool isFound = false;
        int indexOfBeyond = result.p[lmat.i[j]+1];
        double newValue = lmat.a[k]*lmat.a[j]*dvec[i];
        for (int m = result.p[lmat.i[j]]; m < indexOfBeyond; m++) {
          if (result.i[m] == lmat.i[k]) {
            isFound = true;
            result.a[m] += newValue; 
          } else if (result.i[m] > lmat.i[k]) {
            indexOfBeyond = m;
            break;
          }
        }
        if (!isFound) {
          result.i.insert(result.i.begin() + indexOfBeyond, lmat.i[k]);
          result.a.insert(result.a.begin() + indexOfBeyond, newValue);
          for (int n = lmat.i[j]+1; n <= dim; n++)
            result.p[n] += 1;
        }
      }
    }
  }
  return result;
}

void fillWithTranspose(CSCMatrix& lmat) {
  int dim = lmat.p.size() - 1;
  for (int i = 0; i < dim; i++) {
    for (int j = lmat.p[i] + 1; j < lmat.p[i+1]; j++) {
      if (lmat.i[j] <= i)
        continue;
      int k = lmat.p[lmat.i[j]];
      while (k < lmat.p[lmat.i[j]+1] && lmat.i[k] < i)
        k++;
      lmat.i.insert(lmat.i.begin() + k, i);
      lmat.a.insert(lmat.a.begin() + k, lmat.a[j]);
      for (int n = lmat.i[j] + 1; n <= dim; n++)
        lmat.p[n] += 1;
    }
  }
}

CSCMatrix multiplyLDL(const CSCMatrix& lmat, const vector<double>& dvec)
{
  CSCMatrix result = buildLowerTriangle(lmat, dvec);
  fillWithTranspose(result);
  return result;
}

void subtractMatrix(CSCMatrix& amat, const CSCMatrix& bmat) {
  if(amat.p.size() != bmat.p.size()) {
    cout << "subtractMatrix given matrices of different size" << endl;
    exit(EXIT_FAILURE);
  }
  int dim = amat.p.size() - 1;
  for (int i = 0; i < dim; i++) {
    for (int j = bmat.p[i]; j < bmat.p[i+1]; j++) {
      bool isFound = false;
      int indexOfBeyond = amat.p[i+1];
      for (int k = amat.p[i]; k < indexOfBeyond; k++) {
        if (amat.i[k] == bmat.i[j]) {
          isFound = true;
          amat.a[k] -= bmat.a[j];
        } else if (amat.i[k] > bmat.i[j]) {
          indexOfBeyond = k;
          break;
        }
      }
      if (!isFound) {
        amat.i.insert(amat.i.begin() + indexOfBeyond, bmat.i[j]);
        amat.a.insert(amat.a.begin() + indexOfBeyond, -1*bmat.a[j]);
        for (int n = i+1; n <= dim; n++)
          amat.p[n] += 1;
      }
    }
  }
}

double matrix2NormMag(const CSCMatrix& cmat) {
  double count = 0;
  for (size_t i = 0; i < cmat.a.size(); i++)
    count += pow(cmat.a[i], 2);
  return sqrt(count);
}
