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

#include "util.hpp"
#include "repulsion_v2.hpp"

using namespace std;


/// While has any intersections try
/// to resolve them by moving random circle on defined 
/// vector away from overlapping

/// Score has one call operator that returns weight (double)
/// of the circle, how much he resistant to moving anywhere 


template<class Score>
class SA : public ScoreSolver<Score>  {
    using ScoreSolver<Score>::score_;
    
public:
    void MinimumWork(Problem& problem) {
        auto& s = *score_;
        auto& p = problem;
        Count N = p.Size();

        Repulsion_v2<Score> separator;
        separator.set_score(s);
        separator.MinimumWork(p);
        CloseUp(p, s);
        ProblemState S_cur(p);
        ProblemState S_best = S_cur;
        
        uniform_int_distribution<> distr(0, N-1);
        uniform_real_distribution<> rand;
        
        Count iter_count = 10000;
        double temp = 1.; 
        double ddd = pow(10, -1./(iter_count*2));
        for (Index iter = 0; iter < iter_count; ++iter) {
            
            // getting neighbor solution
            double best_work_change = numeric_limits<int>::max();
            int i = distr(RNG);    
            Index best = i;
            for (int j = 0; j < N; ++j) {
                if (i == j) continue;
                double r_0 = p[i].radius;
                double r_1 = p[j].radius;
                Point c_0 = p[i].center();
                Point c_1 = p[j].center();
                // this parameter is important
                if (abs(r_0 - r_1) > (r_0 + r_1)/5.) continue;
                double work_change = p.WorkChange(i, c_1) + p.WorkChange(j, c_0);
                if (work_change < best_work_change) {
                    best_work_change = work_change;
                    best = j;
                }
            }
            p.Swap(i, best);
            if (p.HasOverlap(i) || p.HasOverlap(best)) {
                separator.MinimumWork(p);
            }
            CloseUp(p, s);
            ProblemState S(p);
            double diff = S.work - S_cur.work;
            temp *= ddd;
            double tt = exp(-diff/temp);
            if (diff <= 0) {
                S_cur = S;
                if (S_cur.work < S_best.work) {
                    cerr << "best: " << S_cur.work << endl;
                    S_best = S_cur;
                }
            } else if (tt > rand(RNG)) {
                S_cur = S;
            }
            S_cur.Bring(p);
        }
        S_best.Bring(p);
    }
};




//struct Naive_v1 {
//
//        
//    static void RotateCloseUp(vector<::Circle>& circles) {
//        Field field = Util::ToField(circles);
//        auto dec_density = [](shared_ptr<Field::Circle> p_0, shared_ptr<Field::Circle> p_1) {
//            return p_0->circle().density() > p_1->circle().density();
//        };
//        
//        bool did_close_up = true;
//        while (did_close_up) {
//            did_close_up = false;
//            auto cs = field.circles();
//            sort(cs.begin(), cs.end(), dec_density);
//            for (auto c : cs) {
//                field.remove(c);
//                while (RotateCloseUp(c, field, 3.14/2, 0.00001)) did_close_up = true;
//                field.add(c);
//            }
//        }
//        
//        for (int i = 0; i < circles.size(); ++i) {
//            circles[i].center = field.circle(circles[i].id)->center();
//        }
//    }
//    
//    static double MaxRotation(shared_ptr<Field::Circle> circle, const Field& field, double bound_angle, double eps) {
//        auto& ccc = circle->circle();
//        double a = 0;
//        double b = bound_angle;
//        while (abs(b - a) > eps) {
//            double c = (a + b)/2.;
//            ccc.center = RotatePoint(ccc.center, ccc.origin, c);
//            if (field.hasIntersection(circle)) {
//                b = c;
//            } else {
//                a = c;
//            }
//        }
//        return a;
//    }
//    
//    
//    // did change position of circle
//    static bool RotateCloseUp(shared_ptr<Field::Circle> circle, const Field& field, double max_rotation, double arc_step) {
//        vector<Point> closer_centers;
//        auto& ccc = circle->circle();
//        Point starting_center = ccc.center;
//        double eps = 1e-6;
//        double a_0 = MaxRotation(circle, field, -max_rotation, eps);
//        double a_1 = MaxRotation(circle, field, +max_rotation, eps);
//        double len = ccc.radius * (a_1 - a_0);
//        // want defenetly check last point
//        for (double d = 0; d < len; d+=arc_step) {
//            double a = a_0 + d / ccc.radius;
//            ccc.center = RotatePoint(starting_center, ccc.origin, a);
//            if (field.hasIntersection(circle)) continue;
//            if (CloseUp(circle, ccc.origin, field)) {
//                 closer_centers.push_back(ccc.center);
//            }
//        }
//        if (closer_centers.empty()) {
//            ccc.center = starting_center;
//            return false;
//        }
//        Point min = *min_element(closer_centers.begin(), closer_centers.end(), [&](const Point& p_0, const Point& p_1) {
//            return ccc.origin.distance(p_0) < ccc.origin.distance(p_1);
//        }); 
//        ccc.center = min;
//        if (ccc.origin.distance(starting_center) - ccc.origin.distance(min) < eps) {
//            return false;
//        }
//        return true;    
//
//    }
//    
//    
//    
//};
//
#endif
