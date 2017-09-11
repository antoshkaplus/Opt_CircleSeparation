#pragma once

#include "util.hpp"
#include "field.hpp"


template<class Score>
class Repulsion_v2 {
public:
    vector<Point> MinimumWork(Problem& problem) {
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
                auto& c = cs[i];
                auto ins = field.Intersections(&cs[i]);
                if (ins.empty()) continue;
                has_intersections = true;
                
                /// finding most significabt intersection
                auto m_it = &ins[rand()%ins.size()]; // MaxElement(ins.begin(), ins.end(), score_ptr);
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