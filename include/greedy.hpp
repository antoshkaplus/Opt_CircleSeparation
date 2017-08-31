#pragma once

#include "util.hpp"
#include "field.hpp"


template<class Score>
struct Greedy {
private:
    
    constexpr static const double kCompactedness = 0.01;
    constexpr static const double kGoldenAngle = 2*M_PI*0.381966;
   
public:
    
    vector<Point> MinimumWork(Problem& problem) {
        auto cs = ProblemToCircles(problem);
        for_each(cs.begin(), cs.end(), [](auto& c) { c.radius += RADIUS_EPS; });

        auto field = BuildField(cs);
        field.Clear();

        Order ordering;
        ordering.DoOrder(cs, score);
        
        for (auto i : ordering.get()) {
            auto c = cs.data() + i;
            double angle = 0;
            double distance = 0;
            Index index = 0;
            while (field.HasIntersection(c)) {
                // should be something similar to spiral
                distance = kCompactedness * sqrt(index);
                angle = index * kGoldenAngle;
                c->set_center(Point(distance*cos(angle) + c->center().x, 
                                    distance*sin(angle) + c->center().y));
                ++index;
            }
            field.Add(c);
        }

        assert(ValidArrangement(cs.begin(), cs.end()));

        vector<Point> res(cs.size());
        transform(cs.begin(), cs.end(), res.begin(), [](auto& c) {
            return c.center();
        });
        return res;
    }

    void set_score(Score s) {
        score = move(s);
    }

private:

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

    Score score;
};