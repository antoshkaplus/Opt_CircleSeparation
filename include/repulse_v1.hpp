//
//  repulse_1.hpp
//  CircleSeparation
//
//  Created by Anton Logunov on 2/11/15.
//
//

#ifndef CircleSeparation_repulse_1_hpp
#define CircleSeparation_repulse_1_hpp

#include "naive_v1.hpp"


struct Repulse_1 {

    static void Separate() {
        
        auto max_radius = max_element(circles.begin(), circles.end(), [](::Circle& c_0, ::Circle& c_1) {
            return c_0.radius < c_1.radius;
        })->radius;
        
        auto comp_density = [](::Circle& c_0, ::Circle& c_1) {
            return c_0.density() < c_1.density();
        };
        
        double min_density = min_element(circles.begin(), circles.end(), comp_density)->density();
        double max_density = max_element(circles.begin(), circles.end(), comp_density)->density();        
        
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
            for (auto& c : cs_field) {
                auto ins = field.intersections(c);
                if (ins.empty()) continue;
                has_intersections = true;
                
                
                
                
                
                
                
                
                
                Indent shift;
                double min_overlap = 1;
                double max_overlap = 0;
                for (auto& i : ins) {
                    auto n = c->center() - i->center();
                    min_overlap = min<double>(min_overlap, n.distance());
                    max_overlap = max<double>(max_overlap, n.distance());
                    shift += n;
                } 
                shift = shift.normed();
                shift *= (max_overlap - min_overlap) * (max_density - c->circle()->density()) / (max_density - min_density) + min_overlap;
                
                field.remove(c);
                c->circle()->center += shift;
                field.add(c);
            }   
            shuffle(cs_field.begin(), cs_field.end(), rng);
        }
        
        for (int i = 0; i < circles.size(); ++i) {
            circles[i].center = cs[i]->center;
        }

    
    }

}


#endif
