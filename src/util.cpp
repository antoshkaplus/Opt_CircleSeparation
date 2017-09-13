//
//  util.cpp
//  CirclesSeparation
//
//  Created by Anton Logunov on 6/13/15.
//
//

#include "field.hpp"
#include "util.hpp"

using namespace std;

thread_local default_random_engine RNG;


// close up all random
// close up all in specific order that may be influenced circles.
// but random sounds like workable solution

void IncreaseRadius(vector<::Circle>& cs, double eps) {
    for (auto& c : cs) {
        c.radius += eps;
    }
}

void ReadCircles(istream& in, vector<::Circle>& cs) {
    int N;
    in >> N;
    cs.resize(N);
    for (int i = 0; i < N; i++) {
        in >> cs[i].origin.x;
    }
    for (int i = 0; i < N; i++) {
        in >> cs[i].origin.y;
    }
    for (auto& c : cs) {
        c.set_center(c.origin);
    }
    for (int i = 0; i < N; i++) {
        in >> cs[i].radius;
    }
    for (int i = 0; i < N; i++) {
        in >> cs[i].mass;
    }
}

/// separating by zooming out. 
/// bad strategy because of huge bounds 
void ZoomOut(vector<::Circle>& cs) {
//    Problem problem(cs);
//    double max_X = 0;
//    double max_Y = 0;
//    for (auto& c : cs) {
//        auto inters = problem.field.Intersections(&c);
//        for (auto& c_ptr : inters) {
//            f::Indent seg = c.center() - c_ptr->center();
//            f::Indent need_seg = seg / seg.distance() * (c.radius + c_ptr->radius);
//
//            double X = abs(need_seg.dx / seg.dx);
//            double Y = abs(need_seg.dy / seg.dy);
//
//            if (max_X < X) max_X = X;
//            if (max_Y < Y) max_Y = Y;
//        }
//    }
//    for (auto& c : cs) {
//        ant::geometry::d2::f::Point p = c.center();
//        p.x *= max_X;
//        p.y *= max_Y;
//        c.set_center(p);
//    }
}

