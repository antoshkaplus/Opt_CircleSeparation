#pragma once

#include "util.hpp"


class Score {
public:
    Score() {}
    Score(double mass_pow, double rad_pow) : mass_pow_(mass_pow), rad_pow_(rad_pow) {}

    double operator()(const ::Circle& circle) const {
        return pow(circle.mass, mass_pow_)*pow(circle.radius, rad_pow_);
    }

private:
    double mass_pow_;
    double rad_pow_;

};