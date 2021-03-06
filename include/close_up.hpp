#pragma once

#include <rep/strat.hpp>
#include "field.hpp"



inline bool CloseUp(const Field& field, Circle& c, const Point& target) {
    bool res = false;
    Point
        p_old = c.center(),
        p_a = c.center(),
        p_b = target;

    while (p_a.Distance(p_b) > 1.e-5) {
        c.set_center((p_a + p_b)/2.);

        if (field.HasIntersection(&c)) {
            p_b = c.center();
        } else {
            p_a = c.center();
        }
    }
    c.set_center(p_a);
    if (p_old != c.center()) {
        res = true;
    }
    return res;
}


inline bool CloseUp(Field& field, Circle& c) {
    bool res = false;
    Point
        p_old = c.center(),
        p_a = c.center(),
        p_b = c.origin;

    rep::IntersectionResolver inter_resolver;
    while (p_a.Distance(p_b) > 1.e-5) {
        c.set_center((p_a + p_b)/2.);

        auto res = inter_resolver.Resolve(field, c);
        if (res) {
            if (c.center() != inter_resolver.center() && SqrDistance(c.center(), c.origin) < inter_resolver.sqr_distance()) {
                p_b = c.center();
                continue;
            }
            c.set_center(inter_resolver.center());
            p_a = c.center();
        } else {
            p_b = c.center();
        }
    }
    c.set_center(p_a);
    if (p_old != c.center()) {
        res = true;
    }
    return res;
}

inline void BS_CloseUpAllRandom(vector<Circle>& cs) {
    auto field = BuildField(cs);
    vector<Index> order(cs.size());
    iota(order.begin(), order.end(), 0);
    bool close_up = true;
    while (close_up) {
        close_up = false;
        shuffle(order.begin(), order.end(), RNG);
        for (auto i : order) {
            field.Remove(&cs[i]);
            close_up |= CloseUp(field, cs[i]);
            field.Add(&cs[i]);
        }
    }
}


inline void CloseUpAll_FieldCenter(vector<Circle>& cs) {
    auto field = BuildField(cs);
    Order order;

    order.OrderByScore(cs, [](auto& c) { return c.center().Distance({0.5, 0.5}); });

    bool close_up = true;
    while (close_up) {
        close_up = false;
        for (auto i : order.get()) {
            field.Remove(&cs[i]);
            close_up |= CloseUp(field, cs[i], {0.5, 0.5});
            field.Add(&cs[i]);
        }
    }
}