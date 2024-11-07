#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct TripletMatrix {
  vector<int> i;
  vector<int> j;
  vector<double> a;
};

TripletMatrix readMatlabFile(const string& filename)
{
  TripletMatrix tmat;
  ifstream inFile(filename);
  while (!inFile.eof()) {
    int row;
    inFile >> row;
    if (inFile.eof()) break;
    if (!inFile.good()) {
      cout << "Bad read in input file" << endl;
      exit(EXIT_FAILURE);
    }
    tmat.i.push_back(row - 1);
    int col;
    double value;
    inFile >> col >> value;
    tmat.j.push_back(col - 1);
    tmat.a.push_back(value);
  }
  inFile.close();
  return tmat;
}

void writeMatlabFile(const TripletMatrix& tmat, const vector<double>& dvec, const string& tripletFile, const string& dFile)
{
  cout << "writing out to " << tripletFile << " and " << dFile << endl;
  ofstream f(tripletFile);
  for (size_t k = 0; k < tmat.i.size(); k++) {
    f << tmat.i[k] + 1 << " " << tmat.j[k] + 1 << " " << tmat.a[k] << endl;
  }
  ofstream g(dFile);
  for (size_t k = 0; k < dvec.size() - 1; k++) {
    g << dvec[k] << " ";
  }
  g << dvec[dvec.size()-1];
}

ostream& operator<< (ostream& o, const TripletMatrix& t)
{
  const int dim = t.i.size();
  for (int row = 0; row < dim; row++) {
    o << "( " << t.i[row] << ", " << t.j[row] << ", " << t.a[row] << ")";
    if (row != dim - 1)
      o << endl;
  }
  return o;
}
