//
//  util.hpp
//  CircleSeparation
//
//  Created by Anton Logunov on 2/9/15.
//
//

#ifndef CircleSeparation_util_hpp
#define CircleSeparation_util_hpp


#include <random>
#include <iostream>

#include "ant/geometry/d2.hpp"
#include "ant/grid.hpp"


using namespace std;
using namespace ant;

using namespace ant::grid;
using namespace ant::geometry::d2;

using ant::geometry::d2::f::Point;
using ant::geometry::d2::f::Indent;


extern default_random_engine RNG;
constexpr double RADIUS_EPS = 1e-8;


struct Circle : ant::geometry::d2::f::Circle {
    double mass;
    Point origin;
    
    Circle(){}
    
    Circle(const Point& origin, double radius, double mass)
    :   ant::geometry::d2::f::Circle(origin, radius), 
    mass(mass), 
    origin(origin)  {}
    
    double density() const {
        return mass/Area();
    }
    
    const Point& center() const {
        return ant::geometry::d2::f::Circle::center;
    }
    
    void set_center(const Point& p) {
        ant::geometry::d2::f::Circle::center = p;
    }
    
    bool Intersects(const Circle& p) const {
        return center().Distance(p.center()) < radius + p.radius;
    }
};


class Field : public ParticleGrid<Circle*> {
public:
    using ParticleGrid::ParticleGrid;
    using ParticleGrid::Add;
    
    void Relocate(Circle* circle, const Point& new_center) {
        Remove(circle);
        circle->set_center(new_center);
        Add(circle);
    }
    
    void Shift(Circle* circle, Indent shift) {
        Remove(circle);
        circle->set_center(circle->center() + shift);
        Add(circle);
    }
    
    template<class ForwardIt>
    void Add(ForwardIt b, const ForwardIt& e) {
        while (b != e) {
            Add(&*b++);
            
        }
    }
    
private: 
    Field& operator=(const Field& f) { return *this; }
    
public:
    //    static Field Copy(Field& field) {
    //        Field res;
    //        res.reset(field);
    //        for (auto& fc : field.circles_) {
    //            res.add(shared_ptr<Circle>{new Circle(fc.second->circle())});
    //        }
    //        return res;
    //    }
};


struct Problem {
    Problem(const vector<::Circle>& circles) {
        circles_ = circles;
        auto max_r = max_element(circles.begin(), circles.end(), [](const ::Circle& c_0, const ::Circle& c_1) {
            return c_0.radius < c_1.radius;
        })->radius;
        field.Reset({-3, -3}, {4, 4}, {2*max_r, 2*max_r});
        field.Add(circles_.data(), circles_.data() + circles_.size());
    }
    
    ::Circle& operator[](Index i) {
        return circles_[i];
    }
    const ::Circle& operator[](Index i) const {
        return circles_[i];
    }
    
    auto begin() {
        return circles_.begin();
    }
    auto begin() const {
        return circles_.begin();
    }
    
    auto end() {
        return circles_.end();
    }
    auto end() const {
        return circles_.end();
    }
    
    Count Size() const {
        return circles_.size();
    }
    
    template<class Score>
    void Order(vector<Index>& order, Score& score) {
        static vector<double> ss;
        ss.resize(Size());
        order.resize(Size());
        iota(order.begin(), order.end(), 0);
        transform(order.begin(), order.end(), ss.begin(), 
                  [&](Index i) { return score(circles_[i]); });
        sort(order.begin(), order.end(), [&](Index i_0, Index i_1) {
            return ss[i_0] > ss[i_1];
        });
    }
    
    
public:
    Field field;
private:    
    vector<::Circle> circles_;
};


/// Solver is all about separation of circles
class Solver {
public:
    virtual void MinimumWork(Problem& problem) = 0;
};

template<class Score>
class ScoreSolver : public Solver {
protected:
    
    Score *score_;  
    

public:
    void set_score(Score& score) {
        score_ = &score;
    } 
};

// goal is to minimize work
struct Result {
    Result(const Problem& problem) {
        centers.reserve(problem.Size());
        work = 0;
        for (auto& c : problem) {
            centers.push_back(c.center());
            work += c.mass * c.origin.Distance(c.center());
        }
    }

    

    vector<Point> centers;
    double work;
};


bool BS_CloseUp(Problem& problem, Index i);
vector<double> ConvertToOutput(const Problem& problem);
void IncreaseRadius(vector<::Circle>& cs, double eps);
void ReadCircles(istream& in, vector<::Circle>& cs);



template<class Score>
void CloseUp(Problem& problem, Score& score) {        
    vector<Index> order(problem.Size());
    vector<double> ss(problem.Size());
    iota(order.begin(), order.end(), 0);
    transform(order.begin(), order.end(), ss.begin(), 
              [&](Index i) { return score(problem[i]); });
    sort(order.begin(), order.end(), [&](Index i_0, Index i_1) {
        return ss[i_0] > ss[i_1];
    });
    bool did_close_up = true;
    while (did_close_up) {
        did_close_up = false;
        for (auto i : order) {
            if (BS_CloseUp(problem, i)) {
                did_close_up = true;
            }
        }
    }
}



//
//
//struct Util {
//    
//    // could actually use shared_ptr
//    static Field ToField(vector<::Circle>& cs) {
//        auto max_radius = max_element(cs.begin(), cs.end(), [](::Circle& c_0, ::Circle& c_1) {
//            return c_0.radius < c_1.radius;
//        })->radius;
//        
//        Field f(Point(-3, -3), Point(4, 4), f::Size(2*max_radius, 2*max_radius));
//        for (auto& c : cs) {
//            f.add(SH_F_C(new Field::Circle(c)));
//        }
//        return f;
//    }
//    
//    static void FillCenters(vector<::Circle>& cs, const Field& field) {
//        for (int i = 0; i < cs.size(); ++i) {
//            cs[i].center = field.circle(cs[i].id)->center();
//        }
//    }
//    
//    
//    static double Score(vector<::Circle>& circles) {
//        double s = 0;
//        for (auto& c : circles) {
//            s += c.center.distance(c.origin);
//        }
//        return s;
//    }
//};
//



//
//template<class P, class Cond>
//P LogicalBinarySearch(P start, P bound, const Cond& cond) {
//    P a = start;
//    P b = bound;
//    while (abs(start - ))
//    
//    
//    
//}
//
//

#endif
