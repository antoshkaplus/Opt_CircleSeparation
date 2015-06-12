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
    uniform_real_distribution<> distr(0, 1);
    uniform_real_distribution<> radius_distr(min_radius, max_radius);
    int i = 0;
    for (auto& c : cs) {
        c.id = i++;
        c.mass = distr(rng);
        c.center.set(distr(rng), distr(rng));
        c.origin = c.center;
        c.radius = radius_distr(rng);
    }
    return cs;
}


int main(int argc, const char * argv[])
{
    default_random_engine rng;
    uniform_int_distribution<> distr(0, 450);
    int cs_count = distr(rng) + 50;
    uniform_real_distribution<> radius_distr(sqrt(1./cs_count), sqrt(5./cs_count));
    double max_radius = radius_distr(rng);
    auto cs = test(cs_count, 0, max_radius);
    auto cs_best = cs;
    double score, best_score = numeric_limits<double>::max();
    for (int i = 0; i < 100; ++i) {
        auto cs_copy = cs;
        Repulse_1::Separate(cs_copy);
        Naive_v1::CloseUp(cs_copy);
        if ((score = Util::Score(cs_copy)) < best_score) {
            best_score = score;
            cs_best = cs_copy; 
        }
    }
    cs = cs_best;
    
    ofstream output("solution.txt");
    // origin x, y; center x, y; radius r; mass m; 
    for (auto& c : cs) {
        output  << c.origin.x << " " 
                << c.origin.y << " " 
                << c.center.x << " " 
                << c.center.y << " " 
                << c.radius << " "
                << c.mass << endl;
    }
    return 0;
}

