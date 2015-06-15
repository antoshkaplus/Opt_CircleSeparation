//
//  repulsion.hpp
//  CirclesSeparation
//
//  Created by Anton Logunov on 6/13/15.
//
//

#ifndef CirclesSeparation_repulsion_hpp
#define CirclesSeparation_repulsion_hpp

#include "util.hpp"

template<class Score>
class Repulsion_v1 : public ScoreSolver<Score> {
    using ScoreSolver<Score>::score_;

public:
    void MinimumWork(Problem& problem) {
        auto& score = *score_; 
        auto min_s = score(*MinElement(problem.begin(), problem.end(), score));
        auto max_s = score(*MaxElement(problem.begin(), problem.end(), score));
        
        vector<Index> inds(problem.Size());
        iota(inds.begin(), inds.end(), 0);
        
        auto& field = problem.field;
        bool has_intersections = true;
        while (has_intersections) {
            has_intersections = false;
            shuffle(inds.begin(), inds.end(), RNG);
            for (auto i : inds) {
                auto& c = problem[i];
                auto ins = field.Intersections(&problem[i]);
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
                shift *= (max_overlap - min_overlap) * (max_s - score(c)) / (max_s - min_s) + min_overlap;
                field.Shift(&c, shift);
            }   
        }
    }
};

#endif
