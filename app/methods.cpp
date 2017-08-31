
#include "circles_sep.hpp"
#include "greedy.hpp"
#include "score.hpp"


#ifdef C_SEP_GREEDY

vector<double> CirclesSeparation::minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m) {
    Problem pr(x, y, r, m);

    Greedy<Score> g;
    g.set_score(Score(1, 1));
    auto cs = g.MinimumWork(pr);

    return ToSolution(cs);
}

#endif