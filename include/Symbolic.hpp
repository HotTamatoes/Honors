#pragma once

#include <set>
#include <vector>

using namespace std;

typedef set<int> Set;

ostream& operator<< (ostream& o, const Set& s)
{
  for (auto i = s.begin(); i != s.end(); ++i)
    o << *i << " ";
  return o;
}

vector<Set> initializeSet(const CSCMatrix& cmat)
{
  const int dim = cmat.p.size() - 1;
  vector<Set> result(dim);
  for (int k = 0; k < dim; k++) {
    for (int j = cmat.p[k]; j < cmat.p[k+1]; j++) {
      if (cmat.i[j] < k) continue;
      int idx = cmat.i[j];
      result[k].insert(idx);
    }
  }
  return result;
}

vector<Set> symbolicWithQuotient(const CSCMatrix& cmat)
{
  vector<Set> scriptA = initializeSet(cmat);
  const int dim = scriptA.size();
  vector<Set> scriptE(dim);
  for (int j = 0; j < dim; j++) {
    Set scriptL = scriptA[j];
    for (auto it = scriptE.begin(); it != scriptE.end(); ++it) {
      if (it->find(j) != it->end()) {
        scriptL.insert(it->begin(), it->end());
        it->clear();
      }
    }
    scriptE[j].insert(scriptL.begin(), scriptL.end());
    scriptE[j].erase(j);
    for (auto itA = scriptL.begin(); itA != scriptL.end(); ++itA) {
      if ((*itA) == j) continue;
      for (auto itB = scriptL.begin(); itB != scriptL.end(); ++itB)
        scriptA[*itA].erase(*itB);
    }
    scriptA[j] = scriptL;
  }
  return scriptA;
}

