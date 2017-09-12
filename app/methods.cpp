
#include "circles_sep.hpp"
#include "greedy.hpp"
#include "greedy_v2.hpp"
#include "score.hpp"
#include "repulsion_v1.hpp"
#include "repulsion_v2.hpp"
#include "rep_v3.hpp"
#include "close_up.hpp"
#include "reorder.hpp"
#include "sa_v2.hpp"
#include "sa_v3.hpp"


#ifdef C_SEP_GREEDY

vector<double> CirclesSeparation::minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m) {
    Problem pr(std::move(x), std::move(y), std::move(r), std::move(m));

    Greedy<Score> g;
    g.set_score(Score(1, 0));
    auto cs = g.MinimumWork(pr);

    return ToSolution(cs);
}

#endif

#ifdef C_SEP_GREEDY_2

vector<double> CirclesSeparation::minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m) {
    Problem pr(std::move(x), std::move(y), std::move(r), std::move(m));

    Greedy_v2<Score> g;
    g.set_score(Score(1, 0));
    auto cs = g.MinimumWork(pr);

    return ToSolution(cs);
}

#endif

#ifdef C_SEP_REORDER

vector<double> CirclesSeparation::minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m) {
    Problem pr(std::move(x), std::move(y), std::move(r), std::move(m));

    Reorder g;
    auto cs = g.MinimumWork(pr);

    return ToSolution(cs);
}

#endif

#ifdef C_SEP_EXCHANGE

vector<double> CirclesSeparation::minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m) {

    Problem pr(std::move(x), std::move(y), std::move(r), std::move(m));

    Greedy<Score> g;
    g.set_score(Score(1, 0));
    auto ps = g.MinimumWork(pr);

    auto cs = ProblemToCircles(pr);
    PlaceCircles(cs, ps);

    for_each(cs.begin(), cs.end(), [](auto& c) { c.radius += RADIUS_EPS; });

    Exchange ex;
    Field field = BuildField(cs);
    ex.Improve(field, cs, pr.Work(ps));

    return ToSolution(ExtractCenters(cs));
}

#endif

#ifdef C_SEP_REP_1

vector<double> CirclesSeparation::minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m) {
    Problem pr(std::move(x), std::move(y), std::move(r), std::move(m));

    auto cs = ProblemToCircles(pr);

    Repulsion_v1<Score> g;
    g.set_score(Score(1, 0));
    auto ps = g.MinimumWork(pr);

    PlaceCircles(cs, ps);
    BS_CloseUpAllRandom(cs);

    return ToSolution(ExtractCenters(cs));
}

#endif

#ifdef C_SEP_REP_2

vector<double> CirclesSeparation::minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m) {
    Problem pr(std::move(x), std::move(y), std::move(r), std::move(m));

    auto cs = ProblemToCircles(pr);

    Repulsion_v2<Score> g;
    g.set_score(Score(1, 1));
    auto ps = g.MinimumWork(pr);

    PlaceCircles(cs, ps);

    //BS_CloseUpAllRandom(cs);

    return ToSolution(ExtractCenters(cs));
}

#endif

#ifdef C_SEP_REP_3

vector<double> CirclesSeparation::minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m) {
    Problem pr(std::move(x), std::move(y), std::move(r), std::move(m));

    auto cs = ProblemToCircles(pr);

    Rep_v3<Score> g;
    g.set_score(Score(1, 1));
    auto ps = g.MinimumWork(pr);

    PlaceCircles(cs, ps);

    //BS_CloseUpAllRandom(cs);

    return ToSolution(ExtractCenters(cs));
}

#endif

#ifdef C_SEP_SA_2

vector<double> CirclesSeparation::minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m) {
    Problem pr(std::move(x), std::move(y), std::move(r), std::move(m));

    auto cs = ProblemToCircles(pr);

    SA_v2<Rep_v3_Solver<Score>> sa;
    Rep_v3_Solver<Score> solver;
    solver.set_score(Score(1, 1));
    sa.set_solver(solver);

    auto ps = sa.MinimumWork(pr);

    PlaceCircles(cs, ps);
    BS_CloseUpAllRandom(cs);

    return ToSolution(ExtractCenters(cs));
}

#endif

#ifdef C_SEP_SA_3

vector<double> CirclesSeparation::minimumWork(vector<double> x, vector<double> y, vector<double> r, vector<double> m) {
    Problem pr(std::move(x), std::move(y), std::move(r), std::move(m));

    auto cs = ProblemToCircles(pr);

    SA_v3<Rep_v3_Solver_New_2<Score>> sa;
    Rep_v3_Solver_New_2<Score> solver;
    solver.set_score(Score(1, 1));
    sa.set_solver(solver);

    auto ps = sa.MinimumWork(pr);

    PlaceCircles(cs, ps);
    BS_CloseUpAllRandom(cs);

    return ToSolution(ExtractCenters(cs));
}

#endif