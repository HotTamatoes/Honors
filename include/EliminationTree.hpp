#pragma once
#include <vector>

#include "CSCMatrix.hpp"

using namespace std;

typedef vector<int> EliminationTree;
const int head = -1;

void addRow(const int k, EliminationTree& tree, const CSCMatrix& cmat)
{
  for (int j = cmat.p[k]; j < cmat.p[k+1]; j++) {
    if (cmat.i[j] >= k) break;
    int idx = cmat.i[j];
    while (tree[idx] != head)
      idx = tree[idx];
    if (idx != k) tree[idx] = k;
  }
}

vector<int> findLeafs(const EliminationTree& tree) {
  size_t dim = tree.size();
  vector<int> result;

  vector<bool> isLeaf(dim);
  fill(isLeaf.begin(), isLeaf.end(), true);
  for(size_t i = 0; i < dim - 1; i++) {
      isLeaf[tree[i]] = false; 
  }
  for(size_t j = 0; j < dim; j++) {
    if(isLeaf[j])
      result.push_back(j);
  }
  return result;
}
  /*generate(result.begin(), result.end(), [] {
    static int i = 0;
    return i++;
  });*/

EliminationTree findEliminationTree(const CSCMatrix& cmat)
{
  size_t dim = cmat.p.size() - 1;
  EliminationTree tree(dim);
  fill(tree.begin(), tree.end(), head);
  for (size_t k = 0; k < dim; k++)
    addRow(k, tree, cmat);
  return tree;
}

