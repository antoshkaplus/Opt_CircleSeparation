//
//  Circle.h
//  CirclesSeparation
//
//  Created by Anton Logunov on 6/15/13.
//  Copyright (c) 2013 Anton Logunov. All rights reserved.
//

#ifndef __CirclesSeparation__Circle__
#define __CirclesSeparation__Circle__

#include <iostream>
#include <cmath>

#include "ant/core/core.hpp"
#include "ant/geometry/d2.hpp"

using namespace std;
using namespace ant;
using ant::geometry::d2::f::Point;

struct Circle : ant::geometry::d2::f::Circle {
    double mass;
    Point origin;
    int id;
    
    Circle(){}
    
    Circle(int id, const Point& origin, double radius, double mass)
        :   ant::geometry::d2::f::Circle(origin, radius), 
            mass(mass), 
            origin(origin)  {}
    
    double density() const {
      return mass/Area();
    }
};

#endif /* defined(__CirclesSeparation__Circle__) */





