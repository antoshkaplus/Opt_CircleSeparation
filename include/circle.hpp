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

#include "ant/core.h"
#include "ant/geometry/d2.h"

using namespace std;
using namespace ant;
using ant::geometry::d2::f::Point;

struct Circle : ant::geometry::d2::f::Circle {
    double mass;
    // origin and center
    Point origin;
    int id;
    
    Circle(){}
    
    Circle(int id, double x, double y, double radius, double mass)
    : ant::geometry::d2::f::Circle(Point(x, y), radius), mass(mass), origin(x, y)  {}

    bool isOverlap(const Circle& c) const {
      return radius+c.radius - center.distance(c.center) > 0;
    }
    
    double area() const {
      return radius*radius*M_PI;
    }
    
    double density() const {
      return mass/area();
    }
    
    
//    // how much should increase distance between circles
//    double intersectionDistance(const Circle& c) const {
//        return radius + c.radius - center().distance(c.center());
//    }
};

#endif /* defined(__CirclesSeparation__Circle__) */





