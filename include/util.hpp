//
//  util.hpp
//  CircleSeparation
//
//  Created by Anton Logunov on 2/9/15.
//
//

#ifndef CircleSeparation_util_hpp
#define CircleSeparation_util_hpp

#include "grid.hpp"

using SH_F_C = shared_ptr<Field::Circle>;
using SH_C = shared_ptr<::Circle>;


struct Util {
    
    // could actually use shared_ptr
    static Field ToField(vector<::Circle>& cs) {
        auto max_radius = max_element(cs.begin(), cs.end(), [](::Circle& c_0, ::Circle& c_1) {
            return c_0.radius < c_1.radius;
        })->radius;
        
        Field f(Point(-3, -3), Point(4, 4), f::Size(2*max_radius, 2*max_radius));
        for (auto& c : cs) {
            f.add(SH_F_C(new Field::Circle(c)));
        }
        return f;
    }
    
    static void FillCenters(vector<::Circle>& cs, const Field& field) {
        for (int i = 0; i < cs.size(); ++i) {
            cs[i].center = field.circle(cs[i].id)->center();
        }
    }
    
    
    static double Score(vector<::Circle>& circles) {
        double s = 0;
        for (auto& c : circles) {
            s += c.center.distance(c.origin);
        }
        return s;
    }
};




//
//template<class P, class Cond>
//P LogicalBinarySearch(P start, P bound, const Cond& cond) {
//    P a = start;
//    P b = bound;
//    while (abs(start - ))
//    
//    
//    
//}
//
//

#endif
