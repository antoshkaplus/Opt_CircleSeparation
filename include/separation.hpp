#pragma once

#include <iostream>


//struct CirclesSeparation {
//    vector<double> minimumWork(vector<double>& x, vector<double>& y, vector<double>& r, vector<double>& m) {
//        int N = (int)m.size();
//        vector<::Circle> circles(N);
//        vector<double> result(2*N);
//        for (int i = 0; i < N; i++) {
//            circles[i] = ::Circle(x[i], y[i], r[i]+1.e-9/2, m[i]);
//        }
//        Greedy greedy;
//        greedy.minimumWork(circles);
//        return Greedy::solution(circles);
//    }
//};