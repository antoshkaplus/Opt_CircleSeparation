
#include <iostream>

#include "score.hpp"
#include "util.hpp"
#include "circles_sep.hpp"
#include "greedy.hpp"
#include "greedy_v2.hpp"
#include "rep/rep_v4.hpp"

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

void TestScoreNorm() {

    auto pr = GenerateProblem(200);

    auto cs = ProblemToCircles(pr);

    Score score(1, 1);

    vector<double> ss(cs.size());
    transform(cs.begin(), cs.end(), ss.begin(), score);

    std::nth_element(ss.begin(), ss.end(), ss.begin() + ss.size()/2);
    auto median = ss[ss.size()/2];

    vector<double> start_bad, end_bad;

    std::copy_if(ss.begin(), ss.end(), back_inserter(start_bad), [=](double v) { return v < median / 8; });
    std::copy_if(ss.begin(), ss.end(), back_inserter(end_bad), [=](double v) { return v > median * 8; });

    sort(ss.begin(), ss.end());
    sort(start_bad.begin(), start_bad.end());
    sort(end_bad.begin(), end_bad.end());

    Println(cout, ss);
    Println(cout, start_bad);
    Println(cout, end_bad);
}

void TestRep_v4() {
    auto pr = GenerateProblem(200);

    auto cs = ProblemToCircles(pr);

    rep::Rep_v4 g;
    auto ps = g.MinimumWork(pr);

    cout.precision(11);

//    for (auto s : ToSolution(ps)) {
//        cout << s << endl;
//    }

    for (auto s : g.stats()) {
        cout << s.work << " " << s.intersectionScore << endl;
    }

    cout.flush();
}

#ifndef C_SEP_TESTER

void TestMethod() {
    auto pr = GenerateProblem(200);

    CirclesSeparation cs;
    auto res = cs.minimumWork(pr.x, pr.y, pr.r, pr.m);

    //cout << "Work: " << pr.Work(FromSolutionToPoints(res)) << endl;

    cout.precision(11);

    for (auto s : res) {
        cout << s << endl;
    }
    cout.flush();
}

int main() {

}


#else


int main() {
    TestRep_v4();
}


#endif