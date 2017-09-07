#pragma once

#include <random>
#include <iostream>

#include "ant/geometry/d2.hpp"
#include "ant/grid/grid.hpp"

using namespace std;
using namespace ant;

using namespace ant::grid;
using namespace ant::geometry::d2;

using ant::geometry::d2::f::Point;
using ant::geometry::d2::f::Indent;


extern default_random_engine RNG;
constexpr double RADIUS_EPS = 1e-8;


struct Problem {
    vector<double> x, y, r, m;

    Problem() {}
    Problem(vector<double> x, vector<double> y, vector<double> r, vector<double> m)
        : x(std::move(x)), y(std::move(y)), r(std::move(r)), m(std::move(m)) {}

    Count size() const {
        return x.size();
    }

    double Work(const vector<Point>& ps) const {
        double work = 0;
        for (int i = 0; i < size(); ++i) {
            work += m[i] * Point{x[i], y[i]}.Distance(ps[i]);
        }
        return work;
    }
};


struct Circle : ant::geometry::d2::f::Circle {
    Point origin;
    double mass;

    Circle(){}

    Circle(const Point& origin, double radius, double mass)
        :   ant::geometry::d2::f::Circle(origin, radius),
            origin(origin),
            mass(mass) {}

    double density() const {
        return mass/Area();
    }

    const Point& center() const {
        return ant::geometry::d2::f::Circle::center;
    }

    void set_center(const Point& p) {
        ant::geometry::d2::f::Circle::center = p;
    }

    void reset_center() {
        ant::geometry::d2::f::Circle::center = origin;
    }

    bool Intersects(const Circle& p) const {
        return center().Distance(p.center()) < radius + p.radius;
    }
};

inline vector<Circle> ProblemToCircles(const Problem& pr) {
    vector<Circle> res(pr.size());
    for (auto i = 0; i < pr.size(); ++i) {
        res[i] = {Point{pr.x[i], pr.y[i]}, pr.r[i], pr.m[i]};
    }
    return res;
}

inline void PlaceCircles(vector<Circle>& cs, const vector<Point>& ps) {
    for (auto i = 0; i < cs.size(); ++i) {
        cs[i].set_center(ps[i]);
    }
}

inline vector<Point> ExtractCenters(const vector<Circle>& cs) {
    vector<Point> res(cs.size());
    transform(cs.begin(), cs.end(), res.begin(), [](auto& c) {
        return c.center();
    });
    return res;
}

void IncreaseRadius(vector<::Circle>& cs, double eps);
void ReadCircles(istream& in, vector<::Circle>& cs);
void ZoomOut(vector<::Circle>& cs);

inline vector<double> ToSolution(const vector<Point>& cs) {
    vector<double> s(2*cs.size());
    for (auto i = 0; i < cs.size(); ++i) {
        auto& p = cs[i];
        assert(p.x > -100 && p.x < 100 && p.y > -100 && p.y < 100);
        s[2*i]      = p.x;
        s[2*i+1]    = p.y;
    }
    return s;
}


inline Problem ReadProblem(istream& in) {
    Problem res;
    int sz;
    in >> sz;
    res.x.resize(sz);
    res.y.resize(sz);
    res.r.resize(sz);
    res.m.resize(sz);
    for (int i = 0; i < sz; i++) {
        in >> res.x[i];
    }
    for (int i = 0; i < sz; i++) {
        in >> res.y[i];
    }
    for (int i = 0; i < sz; i++) {
        in >> res.r[i];
    }
    for (int i = 0; i < sz; i++) {
        in >> res.m[i];
    }
    return res;
}


template<class ForwardIt>
bool ValidArrangement(ForwardIt b, ForwardIt e) {
    for (ForwardIt it = b; it != e; ++it) {
        for (ForwardIt it_2 = next(it); it_2 != e; ++it_2) {
            if (it->Intersects(*it_2)) {
                return false;
            }
        }
    }
    return true;
}

//
//template<class Score>
//void CloseUp(Problem& problem, Score& score) {
//    static vector<Index> order;
//    problem.Order(order, score);
//    bool did_close_up = true;
//    while (did_close_up) {
//        did_close_up = false;
//        for (auto i : order) {
//            if (BS_CloseUp(problem, i)) {
//                did_close_up = true;
//            }
//        }
//    }
//}


class Order {
public:
    template<class T, class Score>
    const vector<Index>& DoOrder(const vector<T>& items, const Score& score) {
        auto sz = items.size();
        order_.resize(sz);
        iota(order_.begin(), order_.end(), 0);

        values_.resize(sz);
        transform(order_.begin(), order_.end(), values_.begin(), [&](auto i) { return score(items[i]); });

        sort(order_.begin(), order_.end(), [&](auto i_1, auto i_2) {
            return values_[i_1] > values_[i_2];
        });

        return order_;
    }

    const vector<Index>& get() const {
        return order_;
    }

private:
    vector<Index> order_;
    vector<double> values_;
};


inline Problem GenerateProblem(Count sz, double max_r) {
    vector<double> x(sz);
    vector<double> y(sz);
    vector<double> m(sz);
    vector<double> r(sz);

    uniform_real_distribution<> distr;
    for (auto i = 0; i < sz; ++i) {
        x[i] = distr(RNG);
        y[i] = distr(RNG);
        m[i] = distr(RNG);
        r[i] = max_r * distr(RNG);
    }
    return {std::move(x), std::move(y), std::move(r), std::move(m)};
}


inline Problem GenerateProblem(Count sz) {
    uniform_real_distribution<> max_r_distr(sqrt(1./sz), sqrt(5./sz));
    auto max_r = max_r_distr(RNG);

    return GenerateProblem(sz, max_r);
}


class ProblemGroup {

    static constexpr Count kMax_k_1 = 9;
    static constexpr Count kMax_k_2 = 4;

public:
    static Index getIndex(Count sz, double max_r) {
        Index k_1 = (sz - 50) / 50;
        Index k_2 = max<Index>(max_r * sz - 1, 0);
        return k_1 * kMax_k_2 + k_2;
    }

    static Problem generate(Index group) {
        Index k_1 = group / kMax_k_2;
        Index k_2 = group % kMax_k_2;

        uniform_int_distribution<> sz_distr(0, 50);
        Count sz = 50 + 50 * k_1 + sz_distr(RNG);
        double max_r = (k_2 + 1 + uniform_real_distribution<>()(RNG)) / sz;

        return GenerateProblem(sz, max_r);
    }

    static Count count() {
        return kMax_k_1 * kMax_k_2;
    }

};




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