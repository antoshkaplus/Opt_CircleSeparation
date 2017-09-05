#pragma once

#include "field.hpp"


bool BS_CloseUp(Field& field, Circle& c) {
    field.Remove(&c);
    bool res = false;
    Point
        p_old = c.center(),
        p_a = c.center(),
        p_b = c.origin;

    while (p_a.Distance(p_b) > 1.e-5) {
        c.set_center((p_a + p_b)/2.);
        if (field.HasIntersection(&c)) {
            p_b = c.center();
        } else {
            p_a = c.center();
        }
    }
    c.set_center(p_a);
    if (p_old.Distance(c.center()) > 1.e-5) {
        res = true;
    }
    field.Add(&c);
    return res;
}

void BS_CloseUpAllRandom(vector<Circle>& cs) {
    auto field = BuildField(cs);
    vector<Index> order(cs.size());
    iota(order.begin(), order.end(), 0);
    bool close_up = true;
    while (close_up) {
        close_up = false;
        shuffle(order.begin(), order.end(), RNG);
        for (auto i : order) {
            close_up |= BS_CloseUp(field, cs[i]);
        }
    }
}