//
//  main.cpp
//  CirclesSeparation
//
//  Created by Anton Logunov on 6/7/13.
//  Copyright (c) 2013 Anton Logunov. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <map>
#include <functional>

#include "CircleSeparation.h"

using namespace std;
int main(int argc, const char * argv[])
{
  int N;
  cin >> N;
  vector<double> X(N), Y(N), R(N), M(N);
  for (int i = 0; i < N; i++) {
    cin >> X[i];
  }
  for (int i = 0; i < N; i++) {
    cin >> Y[i];
  }
  for (int i = 0; i < N; i++) {
    cin >> R[i];
  }
  for (int i = 0; i < N; i++) {
    cin >> M[i];
  }

  CirclesSeparation cs;
  vector<double> v = cs.minimumWork(X, Y, R, M);
  for (int i = 0; i < 2*N; i++) {
    printf("%.11f\n", v[i]);
  }
  return 0;
}

