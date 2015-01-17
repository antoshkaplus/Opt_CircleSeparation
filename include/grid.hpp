//
//  grid.h
//  CirclesSeparation
//
//  Created by Anton Logunov on 5/18/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#ifndef __CirclesSeparation__grid__
#define __CirclesSeparation__grid__

#include <iostream>
#include <ant>

#include "Circle.h"

using namespace std;
using namespace ant;
using namespace ant::d2;

using ant::d2::f::Point;


struct Field : ParticleGrid {
    struct Circle : ParticleGrid::Particle, ::Circle {
        bool intersects(const Particle& p) const override {
            const auto& c = static_cast<const Circle&>(p);
            return center().distance(c.center()) < radius + c.radius;
        }
        
        // how much should increase distance between circles
        double intersectionDistance(const Circle& c) const {
            return radius + c.radius - center().distance(c.center());
        }
        // maybe to use position (but it can be confused with Position and left top angle)
        Point center() const override {
            return f::Circle::center;
        }
        void set_center(const Point& p) {
            f::Circle::center = p;
        }
        
    };
    
    Field(double min_x, double min_y, 
          double max_x, double max_y,
          double max_particle_width, 
          double max_particle_height) 
          : ant::d2::ParticleGrid(
                min_x, min_y, 
                max_x, max_y,
                max_particle_width, 
                max_particle_height) {}
    Field() {}
    
    vector<shared_ptr<Circle>> intersections(const shared_ptr<Circle>& p) {
        auto ins = ant::d2::ParticleGrid::intersections(p);
        vector<shared_ptr<Circle>> res(ins.size());
        for (auto i = 0; i < ins.size(); ++i) {
            res[i] = static_pointer_cast<Circle>(ins[i]);
        }
        return res;
    }
};



#endif /* defined(__CirclesSeparation__grid__) */
