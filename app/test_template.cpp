
#include <iostream>

#include "score.hpp"
#include "util.hpp"
#include "circles_sep.hpp"
#include "greedy.hpp"
#include "greedy_v2.hpp"

void TestGreedy() {
    auto pr = GenerateProblem(200);
    Score s(1, 0);

    Println(cout, "greedy");

    Greedy<Score> g_1;
    g_1.set_score(s);
    Println(cout, "1: ", pr.Work(g_1.MinimumWork(pr)));

    Greedy_v2<Score> g_2;
    g_2.set_score(s);
    Println(cout, "2: ", pr.Work(g_2.MinimumWork(pr)));

}


int main() {
    auto pr = GenerateProblem(11);

    CirclesSeparation cs;
    auto res = cs.minimumWork(pr.x, pr.y, pr.r, pr.m);

    cout.precision(11);

    for (auto s : res) {
        cout << s << endl;
    }
    cout.flush();
}
