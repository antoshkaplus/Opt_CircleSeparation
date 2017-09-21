#pragma once

#include "util.hpp"


namespace rep {

struct OneStrat {

    f::Indent operator()(Circle& c, const Circle& c_2) {
        auto t = c.center() - c_2.center();
        t *= (c.radius + c_2.radius + 1.e-8 - t.distance()) / t.distance();
        return t;
    }

};

template<class Score>
struct TwoStrat {

    TwoStrat(Score score) : score_(score) {}

    std::pair<f::Indent, f::Indent> operator()(Circle& c, Circle& c_2) {

        auto t = c.center() - c_2.center();
        auto d = t.distance();
        auto diff = c.radius + c_2.radius - d;
        auto ss = score_(c) + score_(c_2);

        auto shift = t * diff * score_(c) / (d * ss);
        auto shift_2 = - t * diff * score_(c_2) / (d * ss);

        return {shift, shift_2};
    }

private:
    Score score_;
};

// returns bool if resolved.
// should also provide expected deviation or new center locations or many if multiple!!!
struct IntersectionResolver {
    bool Resolve(Field& field, Circle& c) {

        OneStrat strat;

        auto inter = field.FirstIntersection(&c);
        if (!inter) {
            center_ = c.center();
            sqr_distance_ = SqrDistance(center_, c.origin);
            return true;
        }

        auto deviation = strat(c, *inter.value());

        auto old_center = c.center();
        c.move_center(deviation);
        auto res = false;
        if (!field.HasIntersection(&c)) {
            res = true;
            center_ = c.center();
            sqr_distance_ = SqrDistance(center_, c.origin);
        }
        c.set_center(old_center);
        return res;
    }

    Point center() const {
        return center_;
    }

    double sqr_distance() const {
        return sqr_distance_;
    }

private:
    Point center_;
    double sqr_distance_;

};

}