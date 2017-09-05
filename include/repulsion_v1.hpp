#pragma once

#include "util.hpp"
#include "field.hpp"


// the score corresponds to circles
template<class Score>
class Repulsion_v1 {
public:
    vector<Point> MinimumWork(Problem& problem) {
        vector<Circle> cs = ProblemToCircles(problem);

        auto min_s = score_(*MinElement(cs.begin(), cs.end(), score_));
        auto max_s = score_(*MaxElement(cs.begin(), cs.end(), score_));
        
        vector<Index> inds(problem.size());
        iota(inds.begin(), inds.end(), 0);
        
        auto&& field = BuildField(cs);
        bool has_intersections = true;
        while (has_intersections) {
            has_intersections = false;
            shuffle(inds.begin(), inds.end(), RNG);
            for (auto i : inds) {
                auto& c = cs[i];
                auto ins = field.Intersections(&c);
                if (ins.empty()) continue;
                has_intersections = true;
                ::Indent shift;
                double min_overlap = 1;
                double max_overlap = 0;
                for (auto& i : ins) {
                    auto n = c.center() - i->center();
                    min_overlap = min<double>(min_overlap, n.distance());
                    max_overlap = max<double>(max_overlap, n.distance());
                    shift += n;
                } 
                shift = shift.normed();
                shift *= (max_overlap - min_overlap) * (max_s - score_(c)) / (max_s - min_s) + min_overlap;
                field.Shift(&c, shift);
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
