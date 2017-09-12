#pragma once

#include <cmath>

#include "util.hpp"
#include "close_up.hpp"
#include "repulsion_v2.hpp"


using namespace std;


template<class Solver>
class SA_v3 {
public:
    vector<Point> MinimumWork(Problem& pr) {
        sz_distr_ = uniform_int_distribution<>(0, pr.size()-1);

        auto cs = ProblemToCircles(pr);
        for_each(cs.begin(), cs.end(), [](auto& c) { c.radius += RADIUS_EPS; });

        auto field = BuildField(cs);

        vector<Index> change_ids;

        solver_.Solve(field, cs, change_ids);

        Solution cur_sol{ExtractCenters(cs), Work(cs)};
        Solution best_sol = cur_sol;
        Solution new_sol;

        Count iter_count = 3000;

        double temp = 1.;
        double ddd = pow(10, -1./(iter_count*2));

        for (Index iter = 0; iter < iter_count; ++iter) {

            auto change_id = Alter(field, cs);
            change_ids.clear();
            change_ids.push_back(change_id);

            solver_.Solve(field, cs, change_ids);

            new_sol.Init(cs);

            temp *= ddd;

            double diff = new_sol.work - cur_sol.work;
            double tt = exp(-diff/temp);
            if (diff <= 0) {
                cur_sol = new_sol;

                if (cur_sol.work < best_sol.work + CIRCLE_INTERSECTION_EPS) {
                    best_sol = cur_sol;

                    cerr.precision(11);
                    cerr << "iter: " << iter << " best: " << best_sol.work << endl;
                }
            } else if (tt > rand()) {
                cur_sol = new_sol;
            } else {
                PlaceCircles(cs, cur_sol.ps);
                field.Clear();
                field.Add(cs.begin(), cs.end());
            }
        }
        return best_sol.ps;
    }

    void set_solver(Solver& solver) {
        solver_ = solver;
    }

private:

    Index Alter(Field& field, vector<Circle>& cs) {
        auto i = sz_distr_(RNG);
        field.Relocate(&cs[i], cs[i].origin);
        return i;
    }

    uniform_int_distribution<> sz_distr_;
    Solver solver_;
};