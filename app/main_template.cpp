
#include <iostream>
#include <cassert>

#include "util.hpp"
#include "circles_sep.hpp"


int main() {
    auto pr = ReadProblem(cin);

    CirclesSeparation cs;
    auto res = cs.minimumWork(pr.x, pr.y, pr.r, pr.m);

    cout.precision(11);

    for (auto s : res) {
        cout << s << endl;
    }
    cout.flush();
}
