//
//  CircleSeparation.h
//  CirclesSeparation
//
//  Created by Anton Logunov on 5/19/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#ifndef __CirclesSeparation__CircleSeparation__
#define __CirclesSeparation__CircleSeparation__

#include <iostream>

#include "solver.hpp"

struct CirclesSeparation {
    vector<double> minimumWork(vector<double>& x, vector<double>& y, vector<double>& r, vector<double>& m) {
        int N = (int)m.size();
        vector<::Circle> circles(N);
        vector<double> result(2*N);
        for (int i = 0; i < N; i++) {
            circles[i] = ::Circle(x[i], y[i], r[i]+1.e-9/2, m[i]);
        }
        Greedy greedy;
        greedy.minimumWork(circles);
        return Greedy::solution(circles);
    }
};



#endif /* defined(__CirclesSeparation__CircleSeparation__) */
