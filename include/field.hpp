#pragma once

#include "util.hpp"


class Field : public ParticleGrid<Circle*> {
    using Base = ParticleGrid<Circle*>;
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
            Base::Add(&(*(b++)));
        }
    }

private:
    Field& operator=(const Field& f) = delete;

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

inline Field BuildField(vector<Circle>& cs) {
    Field field;
    auto max_r = max_element(cs.begin(), cs.end(), [](auto& c_0, auto& c_1) {
        return c_0.radius < c_1.radius;
    })->radius;
    field.Reset({-3, -3}, {4, 4}, {2*max_r, 2*max_r});
    field.Add(cs.begin(), cs.end());
    return field;
}

inline Field BuildFieldFor(vector<Circle>& cs) {
    Field field;
    auto max_r = max_element(cs.begin(), cs.end(), [](auto& c_0, auto& c_1) {
        return c_0.radius < c_1.radius;
    })->radius;
    field.Reset({-3, -3}, {4, 4}, {2*max_r, 2*max_r});
    return field;
}