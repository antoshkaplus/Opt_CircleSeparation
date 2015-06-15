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
    string input, output, logput;
    if (parser.exists("i")) {
        input = parser.getValue("i");
    } else {
        cerr << "unable to find input file path";
        return 1;
    }
    if (parser.exists("o")) {
        output = parser.getValue("o");
    } else {
        cerr << "unable to find output file path";
        return 1;
    }
    if (parser.exists("log")) {
        logput = parser.getValue("log");
    } else {
        cerr << "unable to find log file path";
        return 1;
    }
    ifstream in(input);
    ofstream out(output);
    ofstream log(logput);
    vector<::Circle> cs;
    ReadCircles(in, cs);
    IncreaseRadius(cs, RADIUS_EPS);
    log << "read values" << endl;
    Problem problem(cs);
    Score<1, -2> score;
    Repulsion_v1<decltype(score)> solver;
    solver.set_score(score);
    log << "trying to solve" << endl;
    solver.MinimumWork(problem);
    cs.assign(problem.begin(), problem.end());
    log << "writing down result" << endl;
    WriteCircles(out, cs);
    return 0;
}

