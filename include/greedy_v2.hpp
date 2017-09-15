#pragma once

#include "util.hpp"
#include "field.hpp"
#include "placement.hpp"


template<class Score>
struct Greedy_v2 {
public:
    
    vector<Point> MinimumWork(const Problem& problem) {
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

        int k = 1;
        while (k < cs.size()) {

            swap(order[k-1], order[k]);
            Solve(field, cs, order);
            auto new_sol = ExtractCenters(cs);
            auto new_sol_work = problem.Work(new_sol);

            if (new_sol_work < sol_work) {
                sol = new_sol;
                sol_work = new_sol_work;
                if (k > 1) {
                    --k;
                } else {
                    ++k;
                }
            } else {
                swap(order[k-1], order[k]);
                ++k;
            }
        }

        return sol;
    }

    void set_score(Score s) {
        score = move(s);
    }

private:

    void Solve(Field& field, vector<Circle>& cs, const vector<Index>& order) {
        ResetCenters(cs);
        field.Clear();
        SpiralPlacement placement(field);
        placement.PlaceAll(cs, order);
    }

    Score score;
};



template<class Score>
struct Greedy_v2_2 {
public:

    vector<Point> MinimumWork(const Problem& problem) {
        auto cs = ProblemToCircles(problem);
        for_each(cs.begin(), cs.end(), [](auto& c) { c.radius += RADIUS_EPS; });

        auto field = BuildField(cs);
        field.Clear();

        Order ordering;
        ordering.DoOrder(cs, score_);

        auto order = ordering.get();

        Solve(field, cs, order);
        auto sol = ExtractCenters(cs);
        auto sol_work = problem.Work(sol);

        uniform_int_distribution<> swap_distr_(0, problem.size()-2);

        auto start_millis = GetMillisCount();
        for (auto cur_iter = 0; cur_iter < max_iter_; ++cur_iter) {
            if (GetMillisCount() - start_millis > millis_) break;

            auto k = swap_distr_(RNG);

            // two times better...
            swap(order[k], order[k+1]);
            Solve(field, cs, order);
            auto new_sol = ExtractCenters(cs);
            auto new_sol_work = problem.Work(new_sol);

            if (new_sol_work < sol_work) {
                sol = new_sol;
                sol_work = new_sol_work;
                cerr << cur_iter << " : " << sol_work << endl;
            } else {
                swap(order[k], order[k+1]);
            }
        }
        return sol;
    }

    void set_score(Score s) {
        score_ = move(s);
    }

    void set_millis(Count millis) {
        millis_ = millis;
    }

    void set_max_iter(Index max_iter) {
        max_iter_ = max_iter;
    }

private:

    void Solve(Field& field, vector<Circle>& cs, const vector<Index>& order) {
        ResetCenters(cs);
        field.Clear();
        SpiralPlacement placement(field);
        placement.PlaceAll(cs, order);
    }

    Score score_;
    Count max_iter_;
    Index millis_;
};