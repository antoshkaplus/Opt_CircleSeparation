#pragma once

#include "util.hpp"
#include "field.hpp"
#include "strat.hpp"


namespace rep {

template<class Score>
class Repulsion_v2 {
public:
    vector<Point> MinimumWork(Problem &problem) {
        vector<Circle> cs = ProblemToCircles(problem);

        Order order;
        auto ordering = order.DoOrder(cs, score_);

        // need most pathetic circles first
        reverse(ordering.begin(), ordering.end());

        auto field = BuildField(cs);
        bool has_intersections = true;
        while (has_intersections) {
            has_intersections = false;
            for (auto i : ordering) {
                auto &c = cs[i];
                auto ins = field.Intersections(&cs[i]);
                if (ins.empty()) continue;
                has_intersections = true;

                /// finding most significabt intersection
                auto m_it = &ins[rand() % ins.size()]; // MaxElement(ins.begin(), ins.end(), score_ptr);
                auto t = c.center() - (**m_it).center();
                t *= (c.radius + (**m_it).radius + 1.e-8 - t.distance()) / t.distance();
                field.Shift(&c, t);
            }
        }

        assert(ValidArrangement(cs.begin(), cs.end()));

        return ExtractCenters(cs);
    }

    void set_score(Score s) {
        score_ = move(s);
    }

private:
    Score score_;
};

template<class Score>
class Repulsion_v2_2 {
public:
    vector<Point> MinimumWork(Problem &problem) {
        vector<Circle> cs = ProblemToCircles(problem);
        for_each(cs.begin(), cs.end(), [](auto &c) { c.radius += RADIUS_EPS; });

        Order order;
        TopLeftComparator comp;
        auto ordering = order.OrderByCompare(cs, [&](auto &c_1, auto &c_2) {
            return comp(c_1.origin, c_2.origin);
        });

        // need most pathetic circles first
        reverse(ordering.begin(), ordering.end());

        OneStrat strat;
        auto field = BuildField(cs);
        bool has_intersections = true;
        while (has_intersections) {
            has_intersections = false;
            for (auto i : ordering) {
                auto &c = cs[i];
                auto ins = field.FirstIntersection(&cs[i]);
                if (!ins) continue;
                has_intersections = true;

                /// finding most significabt intersection
                // MaxElement(ins.begin(), ins.end(), score_ptr);
                auto t = strat(c, *ins.value());
                field.Shift(&c, t);
            }
        }

        assert(ValidArrangement(cs.begin(), cs.end()));

        return ExtractCenters(cs);
    }

    void set_score(Score s) {
        score_ = move(s);
    }

private:
    Score score_;
};

}