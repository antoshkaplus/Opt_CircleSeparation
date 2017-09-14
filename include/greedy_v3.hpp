#pragma once

#include "util.hpp"
#include "field.hpp"
#include "placement.hpp"


template<class Score>
struct Greedy_v3 {
public:

    vector<Point> MinimumWork(const Problem& problem) {
        sz_distr_ = uniform_int_distribution<>(0, problem.size()-1);

        auto cs = ProblemToCircles(problem);
        for_each(cs.begin(), cs.end(), [](auto& c) { c.radius += RADIUS_EPS; });

        auto field = BuildField(cs);
        field.Clear();

        Order ordering;
        ordering.DoOrder(cs, score);

        auto order = ordering.get();

        Solve(field, cs, order);
        auto sol = ExtractCenters(cs);
        auto sol_work = problem.Work(sol);
        cerr << sol_work << endl;
        auto start_millis = GetMillisCount();
        vector<Index> old_order;
        for (auto iter = 0; iter < max_iter_; ++iter) {
            if (GetMillisCount() - start_millis > millis_) {
                break;
            }

            old_order = order;

            Alter(cs, order);
            Solve(field, cs, order);
            auto new_sol = ExtractCenters(cs);
            auto new_sol_work = problem.Work(new_sol);

            if (new_sol_work < sol_work) {
                sol = new_sol;
                sol_work = new_sol_work;
                cerr << "new: " << new_sol_work << endl;
            } else {
                PlaceCircles(cs, sol);
                order = old_order;
            }
        }
        return sol;
    }

    void set_score(Score s) {
        score = move(s);
    }

    void set_millis(Count millis) {
        millis_ = millis;
    }

    void set_max_iter(Index max_iter) {
        max_iter_ = max_iter;
    }

private:

    void Alter(vector<Circle>& cs, vector<Index>& order) {
        for (;;) {
            auto k = sz_distr_(RNG);
            auto i = order[k];
            auto& c = cs[i];
            if (c.origin != c.center()) {
                copy(order.begin(), order.begin()+k, order.begin()+1);
                order[0] = i;
                c.set_center(c.origin);
                break;
            }
        }
    }

    void Solve(Field& field, vector<Circle>& cs, const vector<Index>& order) {
        field.Clear();
        SpiralPlacement placement(field);
        placement.PlaceAll(cs, order);
    }

    uniform_int_distribution<> sz_distr_;
    Score score;
    Index max_iter_;
    Count millis_;
};