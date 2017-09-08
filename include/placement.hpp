#pragma once

#include "util.hpp"
#include "close_up.hpp"


// not responsible for Field state
class SpiralPlacement {

    constexpr static const double kCompactedness = 0.01;
    constexpr static const double kGoldenAngle = 2*M_PI*0.381966;

public:

    SpiralPlacement(Field& field) : field_(&field) {}

    // tells you if replace is successful
    bool Place(Circle* c, Count max_count = numeric_limits<Count>::max()) {
        double angle = 0;
        double distance = 0;
        Index index = 0;
        while (field_->HasIntersection(c) && index < max_count) {
            // should be something similar to spiral
            distance = kCompactedness * sqrt(index);
            angle = index * kGoldenAngle;
            c->set_center(Point(distance*cos(angle) + c->center().x,
                                distance*sin(angle) + c->center().y));
            ++index;
        }
        return index != max_count;
    }

    void PlaceAll(vector<Circle>& cs, const vector<Index>& order) {
        for (auto i : order) {
            auto c = cs.data() + i;
            Place(c);
            field_->Add(c);
        }
    }

private:
    Field* field_;

};


class Exchange {
public:

    // return has to be valid!!!
    void Improve(Field& field, vector<Circle>& cs, double work) {

        SpiralPlacement sp(field);
        for (auto i = 0; i < cs.size(); ++i) {
            for (auto j = i+1; j < cs.size(); ++j) {
                CircleChangeStack stack(cs);
                stack.Push(i, j);

                auto& c_1 = cs[i];
                auto& c_2 = cs[j];
                auto diff_before = c_1.Work() + c_2.Work();
                assert(field.Remove(&c_1));
                assert(field.Remove(&c_2));
                SwapCenters(c_1, c_2);
                auto diff_theory = c_1.Work() + c_2.Work();
                if (SimilarSize(c_1, c_2, 0.8) && work - diff_before + diff_theory + 1e-6 < work) {

                    bool b = sp.Place(&cs[i], 1000);
                    if (!b) {
                        stack.Pop();
                        field.Add(&cs[i]);
                        field.Add(&cs[j]);
                        continue;
                    }
                    CloseUp(field, cs[i]);
                    field.Add(&cs[i]);

                    b = sp.Place(&cs[j], 1000);
                    if (!b) {
                        field.Remove(&cs[i]);
                        stack.Pop();
                        field.Add(&cs[i]);
                        field.Add(&cs[j]);
                        continue;
                    }
                    CloseUp(field, cs[j]);
                    field.Add(&cs[j]);

                    auto diff_new = c_1.Work() + c_2.Work();

                    auto new_work = work - diff_before + diff_new;
                    if (new_work + 1e-6 < work) {
                        work = new_work;
                        continue;
                    }
                    field.Remove(&cs[i]);
                    field.Remove(&cs[j]);
                    stack.Pop();
                    field.Add(&cs[i]);
                    field.Add(&cs[j]);
                } else {
                    stack.Pop();
                    field.Add(&cs[i]);
                    field.Add(&cs[j]);
                }
            }
        }

        assert(ValidArrangement(cs.begin(), cs.end()));
    }
};

