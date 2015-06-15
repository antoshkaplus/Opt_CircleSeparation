//
//  score.hpp
//  CirclesSeparation
//
//  Created by Anton Logunov on 6/13/15.
//
//

#ifndef CirclesSeparation_score_hpp
#define CirclesSeparation_score_hpp

#include "util.hpp"


/// input is to ratios
template<int mass_pow, int radius_pow>
class Score {
public:
    
    double operator()(const ::Circle& circle) const {
        return pow(circle.mass, mass_pow)*pow(circle.radius, radius_pow);
    }

};



#endif
