#pragma once

#include <vector>

using namespace std;
typedef vector<vector<double>> DenseMatrix;

DenseMatrix matrixSix()
{
  DenseMatrix m {
    { 1., 0., 3., 4., 0., 0. },
    { 0., 1., 0., 0., 0., 7. },
    { 3., 0., 1., 0., 5., 2. },
    { 4., 0., 0., 1., 6., 0. },
    { 0., 0., 5., 6., 1., 9. },
    { 0., 7., 2., 0., 9., 1. }
  };
  return m;
}

DenseMatrix matrixFour()
{
  DenseMatrix m {
    { 1.8, 0.9, 0.0, 0.5 },
    { 0.9, 1.1, 0.0, 0.0 },
    { 0.0, 0.0, 1.2, 0.6 },
    { 0.5, 0.0, 0.6, 1.1 }
  };
  return m;
}

DenseMatrix matrixLDLTest()
{
  DenseMatrix m {
    { 1. , 0. , 0. , 0. , 0. , 0.2, 0.2, 0. , 0. , 0. , 0.  },
    { 0. , 1. , 0.2, 0. , 0. , 0. , 0. , 0.2, 0. , 0. , 0.  },
    { 0. , 0.2, 1. , 0. , 0. , 0. , 0. , 0. , 0. , 0.2, 0.2 },
    { 0. , 0. , 0. , 1. , 0. , 0.2, 0. , 0. , 0. , 0.2, 0.  },
    { 0. , 0. , 0. , 0. , 1. , 0. , 0. , 0.2, 0. , 0. , 0.2 },
    { 0.2, 0. , 0. , 0.2, 0. , 1. , 0. , 0. , 0.2, 0.2, 0.  },
    { 0.2, 0. , 0. , 0. , 0. , 0. , 1. , 0. , 0. , 0. , 0.2 },
    { 0. , 0.2, 0. , 0. , 0.2, 0. , 0. , 1. , 0. , 0.2, 0.2 },
    { 0. , 0. , 0. , 0. , 0. , 0.2, 0. , 0. , 1. , 0. , 0.  },
    { 0. , 0. , 0.2, 0.2, 0. , 0.2, 0. , 0.2, 0. , 1. , 0.2 },
    { 0. , 0. , 0.2, 0. , 0.2, 0. , 0.2, 0.2, 0. , 0.2, 1.  }
  };
  return m;
}

DenseMatrix matrixThree()
{
  DenseMatrix m {
    { 25., 15., -5. },
    { 15., 18.,  0. },
    { -5.,  0., 11. }
  };
  return m;
}

template <typename T>
ostream& operator<< (ostream& o, const vector<T>& t)
{
  if (!t.size()) {
    o << "Size zero";
  } else {
    size_t i;
    for (i = 0; i < t.size() - 1; i++)
      o << t[i] << " ";
    o << t[i];
  }
  return o;
}

ostream& operator<< (ostream& o, const DenseMatrix& m)
{
  for (size_t i = 1; i < m.size(); i++) {
    o << m[i];
    if (i != m.size() - 1)
      o << endl;
  }
  return o;
}
