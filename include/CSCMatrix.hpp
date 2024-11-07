#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "DenseMatrix.hpp"
#include "TripletMatrix.hpp"

using namespace std;

struct CSCMatrix {
  vector<int> p;
  vector<int> i;
  vector<double> a;

  CSCMatrix() {
    p.push_back(0);
  }

  CSCMatrix(const CSCMatrix& cmat) {
    p = cmat.p;
    i = cmat.i;
    a = cmat.a;
  }
};

CSCMatrix convertDenseToCSC(const DenseMatrix& dmat)
{
  if(dmat.size() == 0 || dmat.size() != dmat[0].size()) {
    cout << "0 size and non-square matrices are not supported" << endl;
    exit(1);
  }
  CSCMatrix cmat;
  int count = 0;
  int dim = dmat.size();
  for (int col = 0; col < dim; col++) {
    for (int row = 0; row < dim; row++) {
      if (dmat[row][col] != 0) {
        cmat.i.push_back(row);
        cmat.a.push_back(dmat[row][col]);
        count++;
      }
    }
    cmat.p.push_back(count);
  }
  return cmat;
}

CSCMatrix convertTripletToCSC(const TripletMatrix& tmat)
{
  CSCMatrix cmat;

  const int dim = tmat.a.size();
  int count = 0;
  int col = 0;
  for (int idx = 0; idx < dim;) {
    for(; col == tmat.j[idx] && idx < dim; idx++) {
      cmat.i.push_back(tmat.i[idx]);
      cmat.a.push_back(tmat.a[idx]);
      count++;
    }
    col++;
    cmat.p.push_back(count);
  }
  return cmat;
}

TripletMatrix convertCSCToTriplet(const CSCMatrix& cmat)
{
  TripletMatrix tmat;

  const int dim = cmat.a.size();
  int col = 0;
  for (int idx = 0; idx < dim; idx++) {
    while (idx >= cmat.p[col + 1])
      col++;
    tmat.i.push_back(cmat.i[idx]);
    tmat.j.push_back(col);
    tmat.a.push_back(cmat.a[idx]);
  }
  return tmat;
}

DenseMatrix convertCSCToDense(const CSCMatrix cmat) {
  DenseMatrix dmat;
  int dim = cmat.p.size() - 1;
  dmat.resize(dim);
  for(int i = 0; i < dim; i++) {
    dmat[i].resize(dim);
    fill(dmat[i].begin(), dmat[i].end(), 0);
  }
  for(int k = 0; k < dim; k++) {
    for(int j = cmat.p[k]; j < cmat.p[k+1]; j++)
      dmat[cmat.i[j]][k] = cmat.a[j];
  }
  return dmat;
}

ostream& operator<< (ostream& o, const CSCMatrix& cmat)
{
  o << cmat.p << endl;
  o << cmat.i << endl;
  o << cmat.a;
  return o;
}

