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

#include "util.hpp"
#include "score.hpp"
#include "sa.hpp"


vector<::Circle> TestCase(int n, double max_radius) {
    assert(max_radius >= sqrt(1./n) - RADIUS_EPS && max_radius <= sqrt(5./n) + RADIUS_EPS);
    vector<::Circle> cs(n);
    uniform_real_distribution<> distr(0, 1);
    uniform_real_distribution<> radius_distr(0, max_radius);
    for (auto& c : cs) {
        c.mass = distr(RNG);
        c.set_center({distr(RNG), distr(RNG)});
        c.origin = c.center();
        c.radius = radius_distr(RNG);
    }
    return cs;
}


int main(int argc, const char * argv[]) {
    default_random_engine rng;
    uniform_int_distribution<> distr(0, 450);
    int cs_count = distr(rng) + 50;
    uniform_real_distribution<> radius_distr(sqrt(1./cs_count), sqrt(5./cs_count));
    double max_radius = radius_distr(rng);
    auto cs = TestCase(cs_count, max_radius);
    
    Problem problem(cs);
    Score<1, -2> score;
    SA<decltype(score)> solver;
    solver.set_score(score);
    solver.MinimumWork(problem);
    cs.assign(problem.begin(), problem.end());
    
    return 0;
}

