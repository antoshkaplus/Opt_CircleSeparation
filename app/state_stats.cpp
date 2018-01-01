
#include "tbb/tbb.h"
#include "tbb/parallel_for.h"

#include "sa_v2.hpp"
#include "sa_v3.hpp"
#include "hill_climb.hpp"
#include "rep_v3.hpp"
#include "score.hpp"

void HillClimb_VS_SA(Count max_iters, Count millis) {

    Count gs = ProblemGroup::count();
    vector<Count> iters(gs);
    vector<double> score(gs);

    vector<Problem> problems(gs);
    for (auto i = 0; i < gs; ++i) {
        problems[i] = ProblemGroup::generate(i);
    }

    tbb::parallel_for(0, gs, 1, [&](Index group) {
        const Problem pr = problems[group];
        auto cs = ProblemToCircles(pr);

        SA_v3<Rep_v3_Solver_New_2 < Score>> sa;
        Rep_v3_Solver_New_2 <Score> solver;
        solver.set_score(Score(1, 1));
        sa.set_solver(solver);
        sa.set_millis(millis);
        sa.set_max_iter(max_iters);

        auto ps = sa.MinimumWork(pr);
        score[group] = pr.Work(ps);
        iters[group] = sa.get_iter_count();

    });
    Println<Index>(cout, iters, "iterations new 2");
    Println<double>(cout, score, "scores new 2");

    tbb::parallel_for(0, gs, 1, [&](Index group) {

        const Problem pr = problems[group];
        auto cs = ProblemToCircles(pr);

        HillClimb<Rep_v3_Solver_New < Score>> sa;
        Rep_v3_Solver_New <Score> solver;
        solver.set_score(Score(1, 1));
        sa.set_solver(solver);
        sa.set_millis(millis);
        sa.set_max_iter(max_iters);

        auto ps = sa.MinimumWork(pr);
        iters[group] = sa.get_iter_count();
        score[group] = pr.Work(ps);
    });
    Println<Index>(cout, iters, "iterations hill climb");
    Println<double>(cout, score, "scores hill climb");
}


void Best_SA() {
    Count gs = ProblemGroup::count();
    vector<Count> iters(gs);

    tbb::parallel_for(0, gs, 1, [&](Index group) {

        Problem pr = ProblemGroup::generate(group);
        auto cs = ProblemToCircles(pr);

        SA_v3<Rep_v3_Solver_New_2 < Score>> sa;
        Rep_v3_Solver_New_2 <Score> solver;
        solver.set_score(Score(1, 1));
        sa.set_solver(solver);
        sa.set_millis(10000);
        sa.set_max_iter(numeric_limits<Count>::max());

        auto ps = sa.MinimumWork(pr);
        iters[group] = sa.get_iter_count();

    });
    Println<Index>(cout, iters, "iterations new 2");

    tbb::parallel_for(0, gs, 1, [&](Index group) {

        Problem pr = ProblemGroup::generate(group);
        auto cs = ProblemToCircles(pr);

        SA_v3<Rep_v3_Solver_New < Score>> sa;
        Rep_v3_Solver_New <Score> solver;
        solver.set_score(Score(1, 1));
        sa.set_solver(solver);
        sa.set_millis(10000);
        sa.set_max_iter(numeric_limits<Count>::max());

        auto ps = sa.MinimumWork(pr);
        iters[group] = sa.get_iter_count();

    });
    Println<Index>(cout, iters, "iterations new");

    tbb::parallel_for(0, gs, 1, [&](Index group) {

        Problem pr = ProblemGroup::generate(group);
        auto cs = ProblemToCircles(pr);

        SA_v2<Rep_v3_Solver< Score>> sa;
        Rep_v3_Solver<Score> solver;
        solver.set_score(Score(1, 1));
        sa.set_solver(solver);
        sa.set_millis(10000);
        sa.set_max_iter(numeric_limits<Count>::max());

        auto ps = sa.MinimumWork(pr);
        iters[group] = sa.get_iter_count();

    });
    Println<Index>(cout, iters, "iterations");
}


// do I actually need SA??? can I just use hill climbing ???
int main() {
    tbb::task_scheduler_init init(20);

    HillClimb_VS_SA(3000, 10000);
    //Best_SA();
}