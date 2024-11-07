#pragma once

#include <tuple>
#include <vector>
#include "Symbolic.hpp"

using namespace std;

// Input: Full CSCMatrix
// Output: Lower triangle of CSCMatrix
// separating this allows quick access to diagonal
CSCMatrix lowerDiagonal(const CSCMatrix& cmat)
{
  CSCMatrix lowerDiag;
  int dim = cmat.p.size() - 1;
  int ctr = 0;
  for (int col = 0; col < dim; col++) {
    for (int idx = cmat.p[col]; idx < cmat.p[col + 1]; idx++) {
      int row = cmat.i[idx];
      if (col > row)
        continue;
      lowerDiag.i.push_back(row);
      lowerDiag.a.push_back(cmat.a[idx]);
      ctr++;
    }
    lowerDiag.p.push_back(ctr);
  }
  return lowerDiag;
}

// Input: Symbolic Analysis Sets
// Output: Inverted Symbolic Analysis Sets
// Used to figure out with nodes on the assembly tree are leaves (not dependent on another frontal factorization)
vector<Set> findDependent(const vector<Set>& scriptL)
{
  int dim = scriptL.size();
  vector<Set> scriptQ(dim);
  for (int i = 0; i < dim; i++)
    for (auto it = scriptL[i].begin(); it != scriptL[i].end(); ++it)
      scriptQ[*it].insert(i);
  return scriptQ;
}

// Inserts the next column into the l factor the algorithm is building column by column (the workhorse of the algorithm)
void insertColumnToCSC(const int col, const Set& scriptL, 
  const CSCMatrix& mmat, CSCMatrix& lmat, double alpha)
{
  int dim = lmat.p.size() - 1;
  int nNonZero = lmat.p[dim];
  for (int i = dim; i < col + 1; i++) { // resize lmat as needed
    lmat.p.push_back(nNonZero);
  }
  int lStart = lmat.p[col];
  int lCtr = 0;
  int mIndex = mmat.p[col];
  int tooFar = mmat.p[col+1];
  double diag = mmat.a[mIndex];
  for (auto it = scriptL.begin(); it != scriptL.end(); ++it, ++mIndex) {
    if (mIndex >= tooFar) {
      break;
    }
    if (mmat.i[mIndex] != *it){
      --mIndex;
      continue;
    }
    double updateVal = mmat.a[mIndex] / diag;
    if (abs(updateVal) > alpha) {
      lmat.i.insert(lmat.i.begin() + lStart + lCtr, *it);
      lmat.a.insert(lmat.a.begin() + lStart + lCtr, updateVal);
      lCtr++;
    }
  }
  dim = lmat.p.size() - 1;
  for (int i = col + 1; i < dim + 1; i++) {
    lmat.p[i] += lCtr;
  }
}

// inserts the next diagonal value into the d vector
void insertDValueToVec(const int col, const double d, vector<double>& dvec)
{
  int dim = dvec.size();
  for (int i = dim; i < col + 1; i++) // resize dvec as needed
    dvec.push_back(0);
  dvec[col] = d;
}

// Updates the matrix based on the factors from the previous frontal matrix
// modifies if the matrix had a value there before
// inserts if the matrix had a 0 there before
void modifyOrInsert(const int row, const int col, CSCMatrix& mmat, 
  const double delta)
{
  int idx = mmat.p[col];
  while (mmat.i[idx] < row && idx != mmat.p[col + 1]){
    idx++;
  }
  if (mmat.i[idx] > row || idx == mmat.p[col + 1]) {
    mmat.i.insert(mmat.i.begin() + idx, row);
    mmat.a.insert(mmat.a.begin() + idx, -delta);
    int dim = mmat.p.size() - 1;
    for (int i = col + 1; i < dim + 1; i++)
      mmat.p[i]++;
  } else {
    mmat.a[idx] -= delta;
  }
}

// Updates the matrix based on the factors from the previous frontal matrix
// does (M - frontal LDL) one spot at a time
void modifyMMatrixByLDL(const int col, CSCMatrix& mmat, const CSCMatrix& lmat, 
    const double diag)
{
  for (int colIdx = lmat.p[col]; colIdx < lmat.p[col + 1]; colIdx++) {
    for (int rowIdx = colIdx; rowIdx < lmat.p[col + 1]; rowIdx++) {
      double delta = lmat.a[colIdx] * diag  * lmat.a[rowIdx];
      modifyOrInsert(lmat.i[rowIdx], lmat.i[colIdx], mmat, delta);
    }
  }
}

// Performs a single frontal matrix factorization
void singlefrontal(const int col, const Set& scriptL, CSCMatrix& mmat, 
  CSCMatrix& lmat, vector<double>& dvec, double alpha)
{
  insertColumnToCSC(col, scriptL, mmat, lmat, alpha);
  double diag = mmat.a[mmat.p[col]];
  insertDValueToVec(col, diag, dvec);
  modifyMMatrixByLDL(col, mmat, lmat, diag);
}

// Erases the column as a dependent from the dependency list
(makes new leaves the algorithm can choose from)
// See findDependent
void eraseColumnFromSet(const int col, vector<Set>& scriptQ)
{
  int dim = scriptQ.size();
  for (int i = 0; i < dim; i++)
    scriptQ[i].erase(col);
}

//  Picks the next node to perform frontal factorization on using the dependency list
void singleNode(const vector<Set>& scriptL, vector<Set>& scriptQ,
  CSCMatrix& mmat, CSCMatrix& lmat, vector<double>& dvec, double alpha)
{
  int col = 0;
  while (scriptQ[col].size() != 1) // column node is not a leaf
    col++;
  singlefrontal(col, scriptL[col], mmat, lmat, dvec, alpha);
  eraseColumnFromSet(col, scriptQ);
}

// Performs an incomplete LDL factorization given symbolic analysis and the matrix
tuple<CSCMatrix, vector<double>> incompleteLDL(const vector<Set>& scriptL, 
  const CSCMatrix& cmat, double alpha)
{
  int dim = cmat.p.size() - 1;
  CSCMatrix mmat = lowerDiagonal(cmat);
  CSCMatrix lmat;
  vector<double> dvec;
  vector<Set> scriptQ = findDependent(scriptL);
  for (int nodeCtr = 0; nodeCtr < dim; nodeCtr++) {
    singleNode(scriptL, scriptQ, mmat, lmat, dvec, alpha);
  }
  return make_tuple(lmat, dvec);
}
