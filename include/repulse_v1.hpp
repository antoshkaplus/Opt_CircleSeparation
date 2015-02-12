//
//  repulse_1.hpp
//  CircleSeparation
//
//  Created by Anton Logunov on 2/11/15.
//
//

#ifndef CircleSeparation_repulse_1_hpp
#define CircleSeparation_repulse_1_hpp

#include <cmath>

#include "grid.hpp"

using namespace std;

struct Repulse_1 {

    static void Separate(vector<::Circle>& circles) {
        
        auto max_radius = max_element(circles.begin(), circles.end(), [](::Circle& c_0, ::Circle& c_1) {
            return c_0.radius < c_1.radius;
        })->radius;
        
        default_random_engine rng;
        
        vector<shared_ptr<::Circle>> cs(circles.size());
        vector<shared_ptr<Field::Circle>> cs_field(circles.size()); 
        Field field(Point(-3, -3), Point(4, 4), f::Size(2*max_radius, 2*max_radius));
        for (int i = 0; i < circles.size(); ++i) {
            cs[i].reset(new ::Circle(circles[i]));
            cs_field[i].reset(new Field::Circle(cs[i]));
            field.add(cs_field[i]);
        }
        
        bool has_intersections = true;
        while (has_intersections) {
            has_intersections = false;
            for (auto c : cs_field) {
                auto ins = field.intersections(c);
                if (ins.empty()) continue;
                has_intersections = true;
                
                decltype(c) c_2_best;
                Point p_best, p_2_best;
                // want to minimize this value
                double score_best = numeric_limits<double>::max();
                for (auto& c_2 : ins) {
                    auto& cc = *(c->circle());
                    auto& cc_2 = *(c_2->circle());
                    auto to_c = cc.center - cc_2.center;
                    // use eps to get from each other for sure
                    double d = (-to_c.distance() + cc.radius + cc_2.radius + 1e-6) / 2.;
                    Point p = cc.center + to_c.normed() * d;
                    Point p_2 = cc_2.center - to_c.normed() * d;
                    double score = cc.mass * (cc.origin - p).distance()
                                + cc_2.mass * (cc_2.origin - p_2).distance();       
                    if (score < score_best) {
                        c_2_best = c_2;
                        score_best = score;
                        p_best = p;
                        p_2_best = p_2;
                    }       
                }     
                field.remove(c);
                field.remove(c_2_best);
                c->circle()->center = p_best;
                c_2_best->circle()->center = p_2_best;
                field.add(c);
                field.add(c_2_best);
            }   
            shuffle(cs_field.begin(), cs_field.end(), rng);
        }
        
        for (int i = 0; i < circles.size(); ++i) {
            circles[i].center = cs[i]->center;
        }
    }
    
    // min is better
    //static double RelocationScore()
    
    static void RestoreCircles(const vector<shared_ptr<Field::Circle>>& cs, Field& field) {
        while (true) {
            
            
            for (auto c : cs) {
                field.remove(c);
                auto& cc = *(c->circle()); 
                auto ins = field.intersections(c);
                if (ins.empty())
                for (auto c_2 : ins) {
                
                }
                
                
            
            }
            
        
        }
    }
    
    
    

};


#endif
