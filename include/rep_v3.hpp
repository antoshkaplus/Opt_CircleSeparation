#pragma once

#include "util.hpp"
#include "field.hpp"


template<class Score>
class Rep_v3 {
public:
    vector<Point> MinimumWork(Problem& problem) {
        vector<Circle> cs = ProblemToCircles(problem);
        for_each(cs.begin(), cs.end(), [](auto& c) { c.radius += RADIUS_EPS; });

        auto field = BuildField(cs);
        bool has_intersections = true;

        vector<Index> order(cs.size());
        iota(order.begin(), order.end(), 0);

        while (has_intersections) {
            has_intersections = false;
            shuffle(order.begin(), order.end(), RNG);
            for (auto i : order) {
                auto& c = cs[i];
                auto ins = field.Intersections(&cs[i]);
                if (ins.empty()) continue;
                has_intersections = true;

                /// finding most significabt intersection
                auto& c_2 = *(ins[rand()%ins.size()]); // MaxElement(ins.begin(), ins.end(), score_ptr);
                auto t = c.center() - c_2.center();
                auto d = t.distance();
                auto diff = c.radius + c_2.radius - d;
                auto ss = score_(c) + score_(c_2);

                auto shift = t * diff * score_(c) / (d * ss);
                auto shift_2 = - t * diff * score_(c_2) / (d * ss);

                // increases time by too much
                field.Shift(&c, 0.1*shift);
                field.Shift(&c_2, 0.1*shift_2);
            }
        }

        assert(ValidArrangement(cs.begin(), cs.end()));

        return ExtractCenters(cs);
    }

    void set_score(Score s) {
        score_ = move(s);
    }

private:
    Score score_;
};


template<class Score>
class Rep_v3_Solver {
public:
    void Solve(Field& field, vector<Circle>& cs)  {
        bool has_intersections = true;

        vector<Index> order(cs.size());
        iota(order.begin(), order.end(), 0);

        while (has_intersections) {
            has_intersections = false;
            shuffle(order.begin(), order.end(), RNG);
            for (auto i : order) {
                auto& c = cs[i];
                auto ins = field.Intersections(&cs[i]);
                if (ins.empty()) continue;
                has_intersections = true;

                /// finding most significabt intersection
                auto& c_2 = *(ins[rand()%ins.size()]); // MaxElement(ins.begin(), ins.end(), score_ptr);
                auto t = c.center() - c_2.center();
                auto d = t.distance();
                auto diff = c.radius + c_2.radius - d;
                auto ss = score_(c) + score_(c_2);

                auto shift = t * diff * score_(c) / (d * ss);
                auto shift_2 = - t * diff * score_(c_2) / (d * ss);

                field.Shift(&c, shift);
                field.Shift(&c_2, shift_2);
            }
        }

        assert(ValidArrangement(cs.begin(), cs.end()));
    }

    void set_score(Score s) {
        score_ = move(s);
    }

private:
    Score score_;
};



template<class Score>
class Rep_v3_Solver_New {
public:
    void Solve(Field& field, vector<Circle>& cs, vector<Index>& overlaps) {

        if (overlaps.empty()) {
            overlaps.resize(cs.size());
            iota(overlaps.begin(), overlaps.end(), 0);
            shuffle(overlaps.begin(), overlaps.end(), RNG);
        }

        while (!overlaps.empty()) {
            auto i = overlaps.back();
            overlaps.pop_back();

            auto &c = cs[i];
            auto ins = field.Intersections(&cs[i]);
            if (ins.empty()) continue;

            overlaps.push_back(i);

            auto k = rand() % ins.size();
            /// finding most significabt intersection
            overlaps.push_back(ins[k] - cs.data());

            auto &c_2 = *(ins[k]); // MaxElement(ins.begin(), ins.end(), score_ptr);
            auto t = c.center() - c_2.center();
            auto d = t.distance();
            auto diff = c.radius + c_2.radius - d;
            auto ss = score_(c) + score_(c_2);

            auto shift = t * diff * score_(c) / (d * ss);
            auto shift_2 = -t * diff * score_(c_2) / (d * ss);

            field.Shift(&c, shift);
            field.Shift(&c_2, shift_2);

        }

        assert(ValidArrangement(cs.begin(), cs.end()));
    }

    void set_score(Score s) {
        score_ = move(s);
    }

private:
    Score score_;
};


template<class Score>
class Rep_v3_Solver_New_2 {
public:
    void Solve(Field& field, vector<Circle>& cs, vector<Index>& overlaps) {

        if (overlaps.empty()) {
            overlaps.resize(cs.size());
            iota(overlaps.begin(), overlaps.end(), 0);
            shuffle(overlaps.begin(), overlaps.end(), RNG);
            is_overlaps.resize(cs.size());
            fill(is_overlaps.begin(), is_overlaps.end(), true);
        } else {
            is_overlaps.resize(cs.size());
            fill(is_overlaps.begin(), is_overlaps.end(), false);
            for_each(overlaps.begin(), overlaps.end(), [&](Index i) {
                is_overlaps[i] = true;
            });
        }

        while (!overlaps.empty()) {
            auto p = rand()%overlaps.size();
            auto i = overlaps[p];
            SwapBackPop(overlaps, p);

            is_overlaps[i] = false;

            auto &c = cs[i];
            auto ins = field.Intersections(&cs[i]);
            if (ins.empty()) {
                continue;
            }

            overlaps.push_back(i);
            is_overlaps[i] = true;

            auto k = rand() % ins.size();
            /// finding most significabt intersection
            if (!is_overlaps[ins[k] - cs.data()]) {
                overlaps.push_back(ins[k] - cs.data());
                is_overlaps[ins[k] - cs.data()] = true;
            }

            auto &c_2 = *(ins[k]); // MaxElement(ins.begin(), ins.end(), score_ptr);
            auto t = c.center() - c_2.center();
            auto d = t.distance();
            auto diff = c.radius + c_2.radius - d;
            auto ss = score_(c) + score_(c_2);

            auto shift = t * diff * score_(c) / (d * ss);
            auto shift_2 = -t * diff * score_(c_2) / (d * ss);

            field.Shift(&c, shift);
            field.Shift(&c_2, shift_2);

        }

        assert(ValidArrangement(cs.begin(), cs.end()));
    }

    void set_score(Score s) {
        score_ = move(s);
    }

private:
    vector<bool> is_overlaps;
    Score score_;
};
