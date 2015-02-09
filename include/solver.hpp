//
//  solver.h
//  CirclesSeparation
//
//  Created by Anton Logunov on 5/18/14.
//  Copyright (c) 2014 Anton Logunov. All rights reserved.
//

#ifndef __CirclesSeparation__solver__
#define __CirclesSeparation__solver__

#include <iostream>

#include "circle.hpp"
#include "grid.hpp"

using namespace std;
using namespace ant;

struct Solver {
    static vector<double> solution(const vector<Circle>& cs) {
        vector<double> s(2*cs.size());
        for (auto i = 0; i < cs.size(); ++i) {
            auto& p = cs[i].center;
            assert(p.x > -100 && p.x < 100 && p.y > -100 && p.y < 100);
            s[2*i]      = p.x;
            s[2*i+1]    = p.y;
        }
        return s;
    }
    // returns false if something is wrong
    static bool check(const vector<Circle>& cs) {
        for (auto i = 0; i < cs.size(); ++i) {
            for (auto j = i+1; j < cs.size(); ++j) {
                if (cs[i].isOverlap(cs[j])) return false;
            }
        }
        return true;
    }
    
    virtual void minimumWork(vector<Circle>& cs) = 0;
};

struct Greedy : Solver {
private:

    vector<shared_ptr<Field::Circle>> circles_;
    Field field_; 
    // maybe this parameter will be changed
    constexpr static const double kCompactedness = 0.01;
    constexpr static const double kGoldenAngle = 2*M_PI*0.381966;

public:

//    void minimumWork(vector<Circle>& cs) override {
//        vector<shared_ptr<Field::Circle>> sh_cs(cs.size());
//        for (auto i = 0; i < cs.size(); ++i) {
//            sh_cs[i].reset(new Field::Circle());
//            static_cast<Circle&>(*sh_cs[i]) = cs[i];
//        }
//                
//        double max_radius = max_element(cs.begin(), cs.end(), [](const Circle& c_0, const Circle& c_1) {
//            return c_0.radius < c_1.radius;
//        })->radius;
//        
//        double side = 5;
//        double half = (side-1)/2;
//        field_.reset(-half, -half, half+1, half+1, 2*max_radius, 2*max_radius);
//        
//        fillNaive(sh_cs);
//        closeToOrigin(sh_cs);         
//        for (auto i = 0; i < cs.size(); ++i) {
//            cs[i].center = sh_cs[i]->center();
//        }
//        field_.clear();
//    }
    
private:

//    void fillNaive(const vector<shared_ptr<Field::Circle>>& sh_cs) {
//        vector<shared_ptr<Field::Circle>> cs(sh_cs);
//        sort(cs.begin(), cs.end(), [] (
//            const shared_ptr<Field::Circle>& c_0, 
//            const shared_ptr<Field::Circle>& c_1) {
//            
//            return c_0->mass > c_1->mass;
//        });
//        
//        dispose(cs);
//    } 
//    
//    bool close(shared_ptr<Field::Circle>& circle, const Point& target) {
//        bool res = false;
//        field_.remove(circle);
//        Point 
//        p_old = circle->center(),
//        p_a = circle->center(),
//        p_b = target;
//        double dist = target.distance(p_old);
//        while (p_a.distance(p_b) > 1.e-9) {
//            circle->set_center((p_a + p_b)/2.);
//            if (field_.hasIntersection(circle)) {
//                p_b = circle->center();
//            } else {
//                p_a = circle->center();
//            }    
//        }
//        circle->set_center(p_a);
//        if (dist - target.distance(p_a) < 0) {
//            res = true;
//        }
//        field_.add(circle);
//        return res;
//    }
    
//    void closeToOrigin(vector<shared_ptr<Field::Circle>>& cs) {
//        bool again = true;
//        while (again) {
//            again = false;
//            for (auto& c : cs) {
//                if (close(c, c->origin)) again = true;
//            }
//        }
//    }
    
//    void closeToPoint(vector<shared_ptr<Field::Circle>>& cs, Point p) {
//        vector<shared_ptr<Field::Circle>> cs_sorted(cs);
//        sort(cs_sorted.begin(), cs_sorted.end(), [&] (
//            const shared_ptr<Field::Circle>& s_0, 
//            const shared_ptr<Field::Circle>& s_1) {
//            
//            return s_0->center().distance(p) < s_1->center().distance(p);
//        }); 
//        
//        bool again = true;
//        while (again) {
//            again = false;
//            for (auto& c : cs_sorted) {
//                if (close(c, p)) again = true;
//            }
//        }
//        
//    }
//    
    // cs : ordered circles
    // 
//    void dispose(const vector<shared_ptr<Field::Circle>>& cs) {
//        vector<shared_ptr<Field::Circle>> inters;
//        
//        f::Size sz(0.5, 0.5); 
//        f::Rectangle quarter[4] = {
//            {{0. , 0. }, sz},
//            {{0.5, 0. }, sz},
//            {{0. , 0.5}, sz},
//            {{0.5, 0.5}, sz}
//        }; 
//        
//        for (auto i = 0; i < cs.size(); ++i) {
//            auto &c = cs[i];
//            double angle = 0;
//            double distance = 0;
//            Index index = 0;
//            
//            // find quater by angle
//            
//            while (!(inters = field_.intersections(c)).empty()) {
//                // first need to get mini vectors
//                distance = kCompactedness * sqrt(index);
//                angle = index * kGoldenAngle;
//                c->set_center(Point(distance*cos(angle) + c->center().x, 
//                                    distance*sin(angle) + c->center().y));
//                // this arent ready.                    
//                if (c->origin.x )
//                            
//                ++index;
//            }
//            field_.add(c);
//        }
//    } 
};

using namespace ant::geometry::d2::f;

struct PenaltyMethod : Solver {
    
    vector<::Circle> *cs_;
    default_random_engine rng_;
    
    void minimumWork(vector<::Circle>& cs) override {
        cs_ = &cs;
        vector<Index> inds(cs.size());
        iota(inds.begin(), inds.end(), 0);

        double penalty = 1;
        auto norm = [](const Indent& p) {
            return p / p.distance();
        };
        bool overlap = true;
        while (overlap) {
            overlap = false;
            for (Index i : inds) {
                auto overlap_inds = overlaps(i);
                if (overlap_inds.empty()) continue;
                overlap = true;
                Indent pressure_sum;
                double pressure_mass = 0;
                for (Index q : overlap_inds) {
                    pressure_sum += norm(cs[i].center - cs[q].center) * (cs[i].center.distance(cs[q].center) - cs[i].radius - cs[q].radius);
                    pressure_mass += cs[q].mass;
                } 
                Indent gravity = cs[i].mass * (cs[i].origin - cs[i].center);
                Indent shift = (gravity + penalty * pressure_sum) / ( (cs[i].mass + pressure_mass));
                cs[i].center += shift;
            }
            penalty *= 1.00001;
            shuffle(inds.begin(), inds.end(), rng_);
        } 
    }
    
    vector<Index> overlaps(Index k) {
        vector<Index> res;
        auto& cs = *cs_; 
        for (int i = 0; i < cs.size(); ++i) {
            if (i == k) continue;
            if (!cs[k].isOverlap(cs[i])) continue;
            res.push_back(i);
        }
        return res;
    }
};








#endif /* defined(__CirclesSeparation__solver__) */
