#pragma once

#include "util.hpp"
#include "field.hpp"
#include "placement.hpp"


template<class Score>
struct Greedy_v2 {
public:
    
    vector<Point> MinimumWork(const Problem& problem) {
        auto cs = ProblemToCircles(problem);
        for_each(cs.begin(), cs.end(), [](auto& c) { c.radius += RADIUS_EPS; });

        auto field = BuildField(cs);
        field.Clear();

        Order ordering;
        ordering.DoOrder(cs, score);

        auto order = ordering.get();

        Solve(field, cs, order);
        auto sol = ExtractCenters(cs);
        auto sol_work = problem.Work(sol);

        int k = 1;
        while (k < cs.size()) {

            swap(order[k-1], order[k]);
            Solve(field, cs, order);
            auto new_sol = ExtractCenters(cs);
            auto new_sol_work = problem.Work(new_sol);

            if (new_sol_work < sol_work) {
                sol = new_sol;
                sol_work = new_sol_work;
                if (k > 1) {
                    --k;
                } else {
                    ++k;
                }
            } else {
                swap(order[k-1], order[k]);
                ++k;
            }
        }

        return sol;
    }

    void set_score(Score s) {
        score = move(s);
    }

private:

    void Solve(Field& field, vector<Circle>& cs, const vector<Index>& order) {
        ResetCenters(cs);
        field.Clear();
        SpiralPlacement placement(field);
        placement.PlaceAll(cs, order);
    }

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