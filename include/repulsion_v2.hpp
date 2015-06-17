//
//  repulsion_v2.hpp
//  CirclesSeparation
//
//  Created by Anton Logunov on 6/16/15.
//
//

#ifndef CirclesSeparation_repulsion_v2_hpp
#define CirclesSeparation_repulsion_v2_hpp

#include "util.hpp"

template<class Score>
class Repulsion_v2 : public ScoreSolver<Score> {
    using ScoreSolver<Score>::score_;
    
public:
    void MinimumWork(Problem& problem) {
        auto& score = *score_; 
//        auto score_ptr = [&](const Circle* c) {
//            return score(*c);
//        };
        vector<Index> order;
        problem.Order(order, score);
        // need most pathetic circles first
        reverse(order.begin(), order.end());
        
        auto& field = problem.field;
        bool has_intersections = true;
        while (has_intersections) {
            has_intersections = false;
            for (auto i : order) {
                auto& c = problem[i];
                auto ins = field.Intersections(&problem[i]);
                if (ins.empty()) continue;
                has_intersections = true;
                
                /// finding most significabt intersection
                auto m_it = &ins[rand()%ins.size()]; // MaxElement(ins.begin(), ins.end(), score_ptr);
                auto t = c.center() - (**m_it).center();
                t *= (c.radius + (**m_it).radius + 1.e-8 - t.distance()) / t.distance();
                field.Shift(&c, t);
            }   
        }
    }
};


#endif
