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

#include "separation.hpp"

#include "naive_v1.hpp"
#include "repulse_v1.hpp"


vector<::Circle> test(int n, double min_radius, double max_radius) {
    vector<::Circle> cs(n);
    default_random_engine rng;
    uniform_real_distribution<> mass_distr(0, 1);
    uniform_real_distribution<> radius_distr(min_radius, max_radius);
    for (auto& c : cs) {
        c.mass = mass_distr(rng);
        c.center.set(mass_distr(rng), mass_distr(rng));
        c.origin.set(0, 0);
        c.radius = radius_distr(rng);
    }
    return cs;
}




using namespace std;
int main(int argc, const char * argv[])
{
    ofstream output("output.txt");
    auto cs = test(20, 3, 5);
    PenaltyMethod pm;
    pm.minimumWork(cs);
    for (const auto& c : cs) {
        output << c.center.x << " " << c.center.y << endl;
    }
    Repulse_1::Separate(cs);
    output.flush();
    output.close();
    return 0;

//    int N;
//    cin >> N;
//    vector<double> X(N), Y(N), R(N), M(N);
//    for (int i = 0; i < N; i++) {
//    cin >> X[i];
//    }
//    for (int i = 0; i < N; i++) {
//    cin >> Y[i];
//    }
//    for (int i = 0; i < N; i++) {
//    cin >> R[i];
//    }
//    for (int i = 0; i < N; i++) {
//    cin >> M[i];
//    }
//
//    CirclesSeparation cs;
//    vector<double> v = cs.minimumWork(X, Y, R, M);
//    for (int i = 0; i < 2*N; i++) {
//        printf("%.11f\n", v[i]);
//    }
//    return 0;
}

