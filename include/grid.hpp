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
#include "ant/geometry/d2.h"
#include "ant/grid.h"

#include "circle.hpp"

using namespace std;
using namespace ant;
using namespace ant::geometry::d2;

using ant::geometry::d2::f::Point;


struct Field : ant::grid::ParticleGrid {

    struct Circle : ParticleGrid::Particle {
    private:
        shared_ptr<::Circle> circle_;
        
    public:
        Circle() {}
        Circle(shared_ptr<::Circle> circle) : circle_(circle) {}
        
        
        // needed for particle interface to keep track of intersections
        bool intersects(const Particle& p) const override {
            const auto& c = static_cast<const Circle&>(p);
            return center().distance(c.center()) < circle_->radius + c.circle_->radius;
        }
        
        // needed for particle interface to assign cell 
        Point center() const override {
            return circle_->center;
        }
        
        void set_circle(shared_ptr<::Circle> c) {
            circle_ = c;
        }
        
        shared_ptr<::Circle> circle() {
            return circle_;
        }
    };
    
    Field(Point min, Point max,
          f::Size max_particle_size) 
          : ant::grid::ParticleGrid(
                min.x, min.y, 
                max.x, max.y,
                max_particle_size.width, 
                max_particle_size.height) {}
    Field() {}
    
    vector<shared_ptr<Circle>> intersections(const shared_ptr<Circle>& p) const {
        auto ins = ant::grid::ParticleGrid::intersections(p);
        vector<shared_ptr<Circle>> res(ins.size());
        for (auto i = 0; i < ins.size(); ++i) {
            res[i] = static_pointer_cast<Circle>(ins[i]);
        }
        return res;
    }
};



#endif /* defined(__CirclesSeparation__grid__) */
