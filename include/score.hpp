#pragma once

#include "util.hpp"


class Score {
public:
    Score() {}
    Score(double mass_pow, double rad_pow) : mass_pow_(mass_pow), rad_pow_(rad_pow) {}

    double operator()(const ::Circle& circle) const {
        return pow(circle.mass, mass_pow_)/pow(circle.radius, rad_pow_);
    }
    // mass / (factor * rad**2)

private:
    double mass_pow_;
    double rad_pow_;

};

// gives you 0.03 in score improvement (3%)
class Vlad_D_Score {
public:
    Vlad_D_Score() {}
    Vlad_D_Score(double max_rad) : max_rad(max_rad) {}

    double operator()(const ::Circle& circle) const {
        return circle.mass * min(max_rad / circle.radius, 2.);
    }

private:
    double max_rad = 0.5;

};