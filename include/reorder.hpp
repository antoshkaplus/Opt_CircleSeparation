#pragma once

#include "util.hpp"
#include "field.hpp"
#include "score.hpp"


template<class ComputeWork>
double SimilarExchange(const vector<Circle>& cs, vector<Index>& ordering, ComputeWork& compute_work, double work) {

    // should access those guys in more random way
    for (auto i = 0; i < ordering.size(); ++i) {
        for (auto j = i+1; j < ordering.size(); ++j) {
            auto& c_1 = cs[ordering[i]];
            auto& c_2 = cs[ordering[j]];
            if (SimilarSize(c_1, c_2, 0.8)) {
                swap(ordering[i], ordering[j]);
                auto new_work = compute_work(ordering);
                if (new_work + 1e-4 < work) {
                    work = new_work;
                } else {
                    swap(ordering[i], ordering[j]);
                }
            }
        }
    }

    return work;
}


class Reorder {
public:

    vector<Point> MinimumWork(const Problem& problem) {
        auto cs = ProblemToCircles(problem);
        for_each(cs.begin(), cs.end(), [](auto& c) { c.radius += RADIUS_EPS; });

        auto field = BuildField(cs);
        field.Clear();

        Order ordering;
        ordering.DoOrder(cs, Score(1, 0));

        auto order = ordering.get();

        auto compute_work = [&](const vector<Index>& ordering) {
            field.Clear();
            ResetCenters(cs);

            SpiralPlacement placement(field);
            placement.PlaceAll(cs, ordering);

            return problem.Work(ExtractCenters(cs));
        };

        SimilarExchange(cs, order, compute_work, compute_work(order));

        SpiralPlacement placement(field);
        placement.PlaceAll(cs, order);
        return ExtractCenters(cs);
    }

};
