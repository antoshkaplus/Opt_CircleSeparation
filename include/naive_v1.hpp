//
//  naive_v1.hpp
//  CircleSeparation
//
//  Created by Anton Logunov on 2/8/15.
//
//

#ifndef CircleSeparation_naive_v1_hpp
#define CircleSeparation_naive_v1_hpp

#include <cmath>

#include "grid.hpp"
#include "util.hpp"

using namespace std;


struct Naive_v1 {

    static void Separate(vector<::Circle>& circles) {
        vector<int> ids(circles.size());
        for (int i = 0; i < ids.size(); ++i) {
            ids[i] = circles[i].id;
        }
        
        Field field = Util::ToField(circles);
        
        auto comp_density = [](::Circle& c_0, ::Circle& c_1) {
            return c_0.density() < c_1.density();
        };
        
        double min_density = min_element(circles.begin(), circles.end(), comp_density)->density();
        double max_density = max_element(circles.begin(), circles.end(), comp_density)->density();        
        
        default_random_engine rng;
        
        bool has_intersections = true;
        while (has_intersections) {
            has_intersections = false;
            auto cs = field.circles();
            shuffle(cs.begin(), cs.end(), rng);
            for (auto c : cs) {
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
                shift *= (max_overlap - min_overlap) * (max_density - c->circle().density()) / (max_density - min_density) + min_overlap;
                field.shift(c, shift);
            }   
        }
        
        for (int i = 0; i < circles.size(); ++i) {
            circles[i].center = field.circle(circles[i].id)->center();
        }
    }
    
    static void CloseUp(vector<::Circle>& circles) {
        auto comp_density = [](::Circle& c_0, ::Circle& c_1) {
            return c_0.density() < c_1.density();
        };
        
        Field field = Util::ToField(circles);
  
        auto dec_density = [](shared_ptr<Field::Circle> p_0, shared_ptr<Field::Circle> p_1) {
            return p_0->circle().density() > p_1->circle().density();
        };

        bool did_close_up = true;
        while (did_close_up) {
            did_close_up = false;
            auto cs = field.circles();
            sort(cs.begin(), cs.end(), dec_density);
            for (auto c : cs) {
                field.remove(c);
                while (CloseUp(c, c->circle().origin, field)) did_close_up = true;
                field.add(c);
            }
        }
            
        for (int i = 0; i < circles.size(); ++i) {
            circles[i].center = field.circle(circles[i].id)->center();
        }
    }
    
    static void RotateCloseUp(vector<::Circle>& circles) {
        Field field = Util::ToField(circles);
        auto dec_density = [](shared_ptr<Field::Circle> p_0, shared_ptr<Field::Circle> p_1) {
            return p_0->circle().density() > p_1->circle().density();
        };
        
        bool did_close_up = true;
        while (did_close_up) {
            did_close_up = false;
            auto cs = field.circles();
            sort(cs.begin(), cs.end(), dec_density);
            for (auto c : cs) {
                field.remove(c);
                while (RotateCloseUp(c, field, 3.14/2, 0.00001)) did_close_up = true;
                field.add(c);
            }
        }
        
        for (int i = 0; i < circles.size(); ++i) {
            circles[i].center = field.circle(circles[i].id)->center();
        }
    }
    
    // circle should not be inside field
    // returns if center of circle became closer to target
    static bool CloseUp(shared_ptr<Field::Circle>& circle, const Point& target, const Field& field) {
        bool res = false;
        Point 
        p_old = circle->center(),
        p_a = circle->center(),
        p_b = target;
        double dist = target.distance(p_old);
        while (p_a.distance(p_b) > 1.e-5) {
            circle->circle().center = (p_a + p_b)/2.;
            if (field.hasIntersection(circle)) {
                p_b = circle->center();
            } else {
                p_a = circle->center();
            }    
        }
        circle->circle().center = p_a;
        if (dist - target.distance(p_a) > 0) {
            res = true;
        }
        return res;
    }
    
    
    static Point RotatePoint(Point p, Point center, double angle) {
        double s = sin(angle);
        double c = cos(angle);
        Point p_new;
        p.x -= center.x;
        p.y -= center.y;
        p_new.x = p.x * c - p.y * s + center.x;
        p_new.y = p.x * s + p.y * c + center.y;
        return p_new;
    }
    
    static double MaxRotation(shared_ptr<Field::Circle> circle, const Field& field, double bound_angle, double eps) {
        auto& ccc = circle->circle();
        double a = 0;
        double b = bound_angle;
        while (abs(b - a) > eps) {
            double c = (a + b)/2.;
            ccc.center = RotatePoint(ccc.center, ccc.origin, c);
            if (field.hasIntersection(circle)) {
                b = c;
            } else {
                a = c;
            }
        }
        return a;
    }
    
    
    // did change position of circle
    static bool RotateCloseUp(shared_ptr<Field::Circle> circle, const Field& field, double max_rotation, double arc_step) {
        vector<Point> closer_centers;
        auto& ccc = circle->circle();
        Point starting_center = ccc.center;
        double eps = 1e-6;
        double a_0 = MaxRotation(circle, field, -max_rotation, eps);
        double a_1 = MaxRotation(circle, field, +max_rotation, eps);
        double len = ccc.radius * (a_1 - a_0);
        // want defenetly check last point
        for (double d = 0; d < len; d+=arc_step) {
            double a = a_0 + d / ccc.radius;
            ccc.center = RotatePoint(starting_center, ccc.origin, a);
            if (field.hasIntersection(circle)) continue;
            if (CloseUp(circle, ccc.origin, field)) {
                 closer_centers.push_back(ccc.center);
            }
        }
        if (closer_centers.empty()) {
            ccc.center = starting_center;
            return false;
        }
        Point min = *min_element(closer_centers.begin(), closer_centers.end(), [&](const Point& p_0, const Point& p_1) {
            return ccc.origin.distance(p_0) < ccc.origin.distance(p_1);
        }); 
        ccc.center = min;
        if (ccc.origin.distance(starting_center) - ccc.origin.distance(min) < eps) {
            return false;
        }
        return true;    

    }
    
    
    
};

#endif
