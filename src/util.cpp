//
//  util.cpp
//  CirclesSeparation
//
//  Created by Anton Logunov on 6/13/15.
//
//

#include "util.hpp"

using namespace std;

default_random_engine RNG;

bool BS_CloseUp(Problem& problem, Index i) {
    auto& field = problem.field;
    auto& circle = problem[i];
    
    field.Remove(&circle);
    bool res = false;
    Point 
    p_old = circle.center(),
    p_a = circle.center(),
    p_b = circle.origin;
    
    while (p_a.Distance(p_b) > 1.e-5) {
        circle.set_center((p_a + p_b)/2.);
        if (problem.field.HasIntersection(&circle)) {
            p_b = circle.center();
        } else {
            p_a = circle.center();
        }    
    }
    circle.set_center(p_a);
    if (p_old.Distance(circle.center()) > 1.e-5) {
        res = true;
    }
    field.Add(&circle);
    return res;
}

vector<double> ConvertToOutput(const Problem& problem) {
    Count N = problem.Size();
    vector<double> s(2*N);
    for (auto i = 0; i < N; ++i) {
        auto& p = problem[i].center();
        assert(p.x > -100 && p.x < 100 && p.y > -100 && p.y < 100);
        s[2*i]      = p.x;
        s[2*i+1]    = p.y;
    }
    return s;
}

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
    Problem problem(cs);
    double max_X = 0;
    double max_Y = 0;
    for (auto& c : cs) {
        auto inters = problem.field.Intersections(&c);
        for (auto& c_ptr : inters) {
            f::Indent seg = c.center() - c_ptr->center();
            f::Indent need_seg = seg / seg.distance() * (c.radius + c_ptr->radius);
            
            double X = abs(need_seg.dx / seg.dx);
            double Y = abs(need_seg.dy / seg.dy);
            
            if (max_X < X) max_X = X;
            if (max_Y < Y) max_Y = Y;
        }
    }
    for (auto& c : cs) {
        ant::geometry::d2::f::Point p = c.center();
        p.x *= max_X;
        p.y *= max_Y;
        c.set_center(p);
    }
}

