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


extern thread_local default_random_engine RNG;
constexpr double RADIUS_EPS = 1e-9;
constexpr double CIRCLE_INTERSECTION_EPS = 1e-14;


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
        return center().Distance(p.center()) < radius + p.radius - CIRCLE_INTERSECTION_EPS;
    }

    double Work() const {
        return mass*center().Distance(origin);
    }

    friend void SwapCenters(Circle& c_1, Circle& c_2);
};

inline double Work(const vector<Circle>& cs) {
    return accumulate(cs.begin(), cs.end(), 0., [](double total, const Circle& c) {
        return total + c.Work();
    });
}

inline void SwapCenters(Circle& c_1, Circle& c_2) {
    auto p_1 = c_1.center();
    auto p_2 = c_2.center();
    c_1.set_center(p_2);
    c_2.set_center(p_1);
}


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

inline void ExtractCenters(const vector<Circle>& cs, vector<Point>& ps) {
    ps.resize(cs.size());
    transform(cs.begin(), cs.end(), ps.begin(), [](auto& c) {
      return c.center();
    });
}

inline vector<Point> ExtractCenters(const vector<Circle>& cs) {
    vector<Point> res(cs.size());
    ExtractCenters(cs, res);
    return res;
}

inline void ResetCenters(vector<Circle>& cs) {
    for_each(cs.begin(), cs.end(), [](auto& c) { c.reset_center(); });
}

struct CircleChangeStack {

    CircleChangeStack(vector<Circle>& cs) : cs_(&cs) {}

    void Push(Index i, Index j) {
        c_1 = cs_->at(i).center();
        c_2 = cs_->at(j).center();
        i_1 = i;
        i_2 = j;
    }

    void Pop() {
        cs_->at(i_1).set_center(c_1);
        cs_->at(i_2).set_center(c_2);
    }

private:
    vector<Circle>* cs_;
    Point c_1, c_2;
    Index i_1, i_2;
};

// 1 means totally the same
inline bool SimilarSize(const Circle& c_1, const Circle& c_2, double ratio) {
    auto minmax_rad = minmax(c_1.radius, c_2.radius);
    return minmax_rad.first / minmax_rad.second > ratio;
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
        // situation of less than 0 should never happen
        Index k_2 = max<Index>(pow(max_r, 2) * sz - 1, 0);
        return k_1 * kMax_k_2 + k_2;
    }

    static Problem generate(Index group) {
        Index k_1 = group / kMax_k_2;
        Index k_2 = group % kMax_k_2;

        uniform_int_distribution<> sz_distr(0, 50);
        Count sz = 50 + 50 * k_1 + sz_distr(RNG);
        double max_r = sqrt((k_2 + 1 + uniform_real_distribution<>()(RNG)) / sz);

        return GenerateProblem(sz, max_r);
    }

    static Count count() {
        return kMax_k_1 * kMax_k_2;
    }

};

struct Solution {
    vector<Point> ps;
    double work;

    Solution() {}
    Solution(vector<Point> ps, double work) : ps(move(ps)), work(work) {}

    void Init(const vector<Circle>& cs) {
        ExtractCenters(cs, ps);
        work = Work(cs);
    }

    bool is_better(const Solution& s) {
        return work < s.work + CIRCLE_INTERSECTION_EPS;
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