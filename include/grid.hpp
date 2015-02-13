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
using ant::geometry::d2::f::Indent;


struct Field : ant::grid::ParticleGrid {
public:
    struct Circle : ParticleGrid::Particle {
    private:
        ::Circle circle_;
        
    public:
        Circle() {}
        Circle(::Circle circle) : circle_(circle) {}
        
        
        // needed for particle interface to keep track of intersections
        bool intersects(const Particle& p) const override {
            const auto& c = static_cast<const Circle&>(p);
            return center().distance(c.center()) < circle_.radius + c.circle_.radius;
        }
        
        // needed for particle interface to assign cell 
        Point center() const override {
            return circle_.center;
        }
        
        void set_circle(::Circle c) {
            circle_ = c;
        }
        
        ::Circle& circle() {
            return circle_;
        }
    };
    
private:
    map<int, shared_ptr<Circle>> circles_;
    
public:
    Field(Point min, Point max,
          f::Size max_particle_size) 
          : ant::grid::ParticleGrid(
                min.x, min.y, 
                max.x, max.y,
                max_particle_size.width, 
                max_particle_size.height) {}
    Field() {}
    
    void add(const std::shared_ptr<Particle>& p) {
        shared_ptr<Circle> pp = static_pointer_cast<Circle>(p);
        circles_[pp->circle().id] = pp;
        ParticleGrid::add(p);
    }
    
    bool remove(const std::shared_ptr<Particle>& p) {
        shared_ptr<Circle> pp = static_pointer_cast<Circle>(p);
        circles_.erase(pp->circle().id);
        return ParticleGrid::remove(p);
    }
    
    void clear() {
        circles_.clear();
    }
    
    void relocate(shared_ptr<Circle> circle, Point new_center) {
        remove(circle);
        circle->circle().center = new_center;
        add(circle);
    }
    
    void shift(shared_ptr<Circle> circle, Indent shift) {
        remove(circle);
        circle->circle().center += shift;
        add(circle);
    }
    
    vector<shared_ptr<Circle>> intersections(const shared_ptr<Circle>& p) const {
        auto ins = ant::grid::ParticleGrid::intersections(p);
        vector<shared_ptr<Circle>> res(ins.size());
        for (auto i = 0; i < ins.size(); ++i) {
            res[i] = static_pointer_cast<Circle>(ins[i]);
        }
        return res;
    }
    
    shared_ptr<Circle> circle(int id) {
        return circles_.at(id);
    }
    
    shared_ptr<Circle> circle(int id) const {
        return circles_.at(id);
    }

    
    vector<shared_ptr<Circle>> circles() const {
        vector<shared_ptr<Circle>> res;
        for (auto& c : circles_) {
            res.emplace_back(new Circle(c.second->circle()));
        }
        return res;
    }
    
private: 
    Field& operator=(const Field& f) { return *this; }

public:
    static Field Copy(Field& field) {
        Field res;
        res.reset(field);
        for (auto& fc : field.circles_) {
            res.add(shared_ptr<Circle>{new Circle(fc.second->circle())});
        }
        return res;
    }
};



#endif /* defined(__CirclesSeparation__grid__) */
