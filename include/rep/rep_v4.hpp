#pragma once

#include "util.hpp"
#include "state_stats.hpp"

namespace rep {

class Rep_v4 {
public:

    // returned solution is not supposed to be feasible
    vector<Point> MinimumWork(Problem& problem) {
        vector<Circle> cs = ProblemToCircles(problem);
        for_each(cs.begin(), cs.end(), [](auto &c) { c.radius += RADIUS_EPS; });

        score = Score(1, 1);
        transform(cs.begin(), cs.end(), back_inserter(scores), score);
        min_score = *min_element(scores.begin(), scores.end());
        max_score = *max_element(scores.begin(), scores.end());

        auto field = BuildField(cs);

        // order doesn't matter

        stats_.emplace_back(field, cs);


        vector<Index> indexes(cs.size());
        iota(indexes.begin(), indexes.end(), 0);

        double coeff = 100;
        while (coeff > 1e-6) {

            for (auto iter = 0; iter < 2000; ++iter) {
                // would be cool to have forEach on particleGrid
                shuffle(indexes.begin(), indexes.end(), RNG);
                for (auto i : indexes) {
                    auto& c = cs[i];

                    auto ins = field.Intersections(&c);

                    auto shift = ComputeShift_5(c, ins, coeff);

                    auto new_loc = c.center() + shift;
                    new_loc.x = max(new_loc.x, field.min_point().x+1e-3);
                    new_loc.x = min(new_loc.x, field.max_point().x-1e-3);

                    new_loc.y = max(new_loc.y, field.min_point().y+1e-3);
                    new_loc.y = min(new_loc.y, field.max_point().y-1e-3);

                    field.Relocate(&c, new_loc);
                }
            }
            stats_.emplace_back(field, cs);

            coeff /= 2;
        }

        return ExtractCenters(cs);
    }

    void set_term(const Term& term) {
        term_ = term;
    }

    const vector<StateStats>& stats() const {
        return stats_;
    }

private:

    // expect to be less than one
    ::Indent ComputeShift_1(Circle& c, const std::vector<Circle*>& ins, double coeff) {

        ::Indent shift;
        if (ins.empty())
        {
            shift = coeff * c.mass * (c.origin - c.center()) / c.radius;
        } else {
            for (auto& i : ins) {
                auto& cIns = *i;

                auto ccDist = c.center().Distance(i->center());
                ::Indent dir;
                if (ccDist < 1.e-6) dir = ::Indent::RandomNormed(RNG);
                else dir = (c.center() - i->center()) / ccDist;

                shift += coeff * dir * (c.radius + i->radius - ccDist) * cIns.mass / cIns.radius;
            }
        }
        return shift;
    }

    ::Indent ComputeShift_2(Circle& c, const std::vector<Circle*>& ins, double coeff) {
        ::Indent shift;

        for (auto& i : ins) {
            auto& cIns = *i;

            auto ccDist = c.center().Distance(i->center());
            ::Indent dir;
            if (ccDist < 1.e-6) dir = ::Indent::RandomNormed(RNG);
            else dir = (c.center() - i->center()) / ccDist;

            shift += coeff * dir * (c.radius + i->radius - ccDist) * (cIns.mass / cIns.radius / (cIns.mass/cIns.radius + c.mass/c.radius));
        }
        if ((c.origin - c.center()).Distance() > 1e-6) {
            if (ins.empty()) {
                shift = coeff * (c.origin - c.center()).normed() * c.radius;
            } else {
                shift += 1. / 2 * (c.origin - c.center()).normed() * shift.Distance();
            }
        }
        return shift;
    }

    ::Indent ComputeShift_3(Circle& c, const std::vector<Circle*>& ins, double coeff) {
        ::Indent shift;

        if (!ins.empty()) {
            Score score(1, 1);

            auto s_c = score(c);

            double s = 0;
            for (auto &i : ins) {
                s = max(s, score(*i));
            }

            shift = coeff * ::Indent::RandomNormed(RNG) * s / (s + s_c);
        }
        ::Indent back_force;
        if ((c.origin - c.center()).Distance() > 1e-6) {
            back_force = (c.origin - c.center()).normed() * min((c.origin - c.center()).Distance(), coeff / 4);
        }
        return shift + back_force;
    }

    ::Indent ComputeShift_4(Circle& c, const std::vector<Circle*>& ins, double coeff) {
        ::Indent shift;

        if (!ins.empty()) {

            Score score(1, 2);

            auto s_c = score(c);

            double s = 0;
            for (auto &i : ins) {
                s += score(*i);
            }

            uniform_real_distribution<> distr;
            if (distr(RNG) < 0.6) {
                shift = coeff * ::Indent::RandomNormed(RNG) * s / (s + s_c);
            }
            else {
                shift = coeff * (c.center() - ::Point{0.5, 0.5}).normed() * s / (s + s_c);
            }



            if ((c.origin - c.center()).Distance() > 1e-6) {

                shift += (c.origin - c.center()).normed() * coeff / 4;

                // not really
                //shift += coeff * (c.origin - c.center()).normed() * s_c / (s + s_c) ;

            }

            return shift;

        } else {

            if ((c.origin - c.center()).Distance() > 1e-6) {
                return (c.origin - c.center()).normed() * min((c.origin - c.center()).Distance(), coeff / 2);
            }
            else {
                return {};
            }
        }
    }

    ::Indent ComputeShift_5(Circle& c, const std::vector<Circle*>& ins, double coeff) {
        ::Indent shift;

        if (!ins.empty()) {

            auto s_c = score(c);

            double s = 0;
            for (auto &i : ins) {
                s = max(s, score(*i));
            }

            uniform_real_distribution<> distr;
            if (distr(RNG) < 1) {
                shift = coeff * ::Indent::RandomNormed(RNG) * s / (s + s_c);
            }
            else {
                // doesn't work due to can't come back if some circles got stuck
                //shift = coeff * (c.center() - ::Point{0.5, 0.5}).normed() * s / (s + s_c);
            }

            if ((c.origin - c.center()).Distance() > 1e-6) {

                shift += (c.origin - c.center()).normed() * min((c.origin - c.center()).Distance(),  coeff * s_c / max_score);

            }

            return shift;

        } else {

            if ((c.origin - c.center()).Distance() > 1e-6) {
                return (c.origin - c.center()).normed() * min((c.origin - c.center()).Distance(), coeff / 2);
            }
            else {
                return {};
            }
        }
    }



    ::Indent force(Circle& c, double coeff) {
        return coeff * c.mass * (c.origin - c.center()) / c.radius;
    }

    Score score;
    vector<double> scores;
    double min_score;
    double max_score;

    vector<StateStats> stats_;
    Term term_;
};

}