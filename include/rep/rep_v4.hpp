#pragma once

#include "util.hpp"

namespace rep {

class Rep_v4 {
public:

    // returned solution is not supposed to be feasible
    vector<Point> MinimumWork(Problem& problem) {
        vector<Circle> cs = ProblemToCircles(problem);
        for_each(cs.begin(), cs.end(), [](auto &c) { c.radius += RADIUS_EPS; });

        auto field = BuildField(cs);

        // order doesn't matter

        double coeff = 1;
        while (coeff > 1e-7) {

            for (auto iter = 0; iter < 2000; ++iter) {
                // would be cool to have forEach on particleGrid
                for (auto& c : cs) {

                    auto ins = field.Intersections(&c);


                    ::Indent shift = coeff * c.mass * (c.origin - c.center()) / c.radius;
                    for (auto& i : ins) {
                        auto iGrav = i->origin - i->center();




                        double dot = DotProduct(iGrav, c.center() - i->center()); // we divide by distance between centers...
                        // but c.radious + i->radious - distance(c.center() - i->center())
                        if (dot < 0) continue;



                        auto ccDist = c.center().Distance(i->center());
                        Indent dir;
                        if (ccDist < 1.e-3) dir = Indent::RandomNormed();
                        else dir = (c.center() - i->center()) / ccDist;

                        shift += dot / c.center() - i->center()



                        shift +=  force(*i, coeff);
                    }
                }
            }
            coeff /= 2;
        }

    }

    void set_term(const Term& term) {
        term_ = term;
    }

private:

    Indent force(Circle& c, double coeff) {
        return coeff * c.mass * (c.origin - c.center()) / c.radius;
    }

    Term term_;
};

}