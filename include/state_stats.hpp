#pragma once

#include "field.hpp"

struct StateStats {

    StateStats() {}

    StateStats(const Field& field, vector<Circle>& cs) {
        for (auto& c : cs) {
            work += c.Work();
            auto ins = field.Intersections(&c);
            double localScore = 0;
            for (auto i : ins) {
                localScore += pow(c.radius + i->radius - c.center().Distance(i->center()), 2);
            }
            intersectionScore += sqrt(localScore);
        }
    }

    double work = 0;
    double intersectionScore = 0;

};