//
//  main.cpp
//  Test
//
//  Created by Anton Logunov on 5/19/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#include <iostream>
#include <random>

#include <ant>

#include "CircleSeparation.h"

using namespace ant;
using namespace std;

int main(int argc, const char * argv[])
{
    Count n = 130;
    default_random_engine rng;
    uniform_real_distribution<double> distr_all(0, 1);
    uniform_real_distribution<double> distr_rad(sqrt(1./n), sqrt(5./n));
    
    
    vector<double> ms(n);
    vector<double> xs(n);
    vector<double> ys(n);
    vector<double> rs(n);
    
    auto gen_all = bind(distr_all, rng);
    generate(ms.begin(), ms.end(), gen_all);
    generate(xs.begin(), xs.end(), gen_all);
    generate(ys.begin(), ys.end(), gen_all);
    auto gen_rad = bind(distr_rad, rng);
    generate(rs.begin(), rs.end(), gen_rad);
    
    CirclesSeparation cs;
    cs.minimumWork(xs, ys, rs, ms);

    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}

