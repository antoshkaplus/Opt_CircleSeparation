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

#include "ant/core/core.hpp"
#include "repulsion_v1.hpp"
#include "score.hpp"
#include "sa.hpp"


void Write(ostream& out, const vector<double>& ds) {
    for (auto d : ds) {
        printf("%.11f\n", d);
    }
}

void WriteCircles(ostream& out, const vector<::Circle>& cs) {
    out.precision(11);
    for (int i = 0; i < cs.size(); i++) {
        auto p = cs[i].center();
        out << p.x << endl << p.y << endl;
    }
    out.flush();
}

using namespace std;
int main(int argc, const char * argv[])
{
    command_line_parser parser(argv, argc);
    vector<::Circle> cs;
    ReadCircles(cin, cs);
    IncreaseRadius(cs, RADIUS_EPS);
    cerr << "read values" << endl;
    Problem problem(cs);
    // Score<1, -2> score;
     Score<1, -2> score; // best
    // Score<2, -3> score;
    SA<decltype(score)> solver;
    solver.set_score(score);
    cerr << "trying to solve" << endl;
    solver.MinimumWork(problem);
    cerr << "final work: " << problem.Work() << endl;
    cs.assign(problem.begin(), problem.end());
    cerr << "writing down result" << endl;
    WriteCircles(cout, cs);
    return 0;
}

