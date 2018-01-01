#pragma once


class TangentPlacement_1_ToOrigin {
public:
    void PlaceAll(vector<Circle>& cs, const vector<Index>& order) {
        auto max_rad = MaxRadius(cs);

        Field field = BuildFieldFor(cs);
        for (auto i : order) {
            auto& c = cs[i];
            c.set_center(c.origin);
            if (field.HasIntersection(&c)) {
                Point best_center;
                auto best_sqr_distance = numeric_limits<double>::max();
                for (auto j : order) {
                    if (i == j) break;

                    auto dist = c.radius + cs[j].radius + RADIUS_EPS;
                    for (auto phi = 0.; phi < 2*M_PI; phi += max_rad * 0.02) {
                        auto p = ant::geometry::d2::f::FromPolar({dist, phi}) + f::Indent(cs[j].center());
                        c.set_center(p);
                        if (!field.HasIntersection(&c) && best_sqr_distance > SqrDistance(p, c.origin)) {
                            best_sqr_distance = SqrDistance(p, c.origin);
                            best_center = p;
                        }
                    }
                }
                assert(best_sqr_distance != numeric_limits<double>::max());
                c.set_center(best_center);
            }
            field.Add(&c);
        }
    }
};

class TangentPlacement_1_ToCenter {
public:
    void PlaceAll(vector<Circle>& cs, const vector<Index>& order) {
        auto ps = ExtractCenters(cs);

        auto max_rad = MaxRadius(cs);

        Field field = BuildFieldFor(cs);
        for (auto i : order) {
            auto& c = cs[i];
            c.set_center(c.origin);
            if (field.HasIntersection(&c)) {

                Point best_center;
                auto best_center_distance = numeric_limits<double>::max();

                for (auto j : order) {
                    if (i == j) break;

                    auto dist = c.radius + cs[j].radius + RADIUS_EPS;
                    for (auto phi = 0.; phi < 2*M_PI; phi += max_rad * 0.02) {
                        auto p = ant::geometry::d2::f::FromPolar({dist, phi}) + f::Indent(cs[j].center());
                        c.set_center(p);
                        if (!field.HasIntersection(&c) && best_center_distance > SqrDistance(p, c.center())) {
                            best_center_distance = SqrDistance(p, c.center());
                            best_center = p;
                        }
                    }
                }
                assert(best_sqr_distance != numeric_limits<double>::max());
                c.set_center(best_center);
            }
            field.Add(&c);
        }
    }
};

class TangentPlacement_2_ToOrigin {
public:
    void PlaceAll(vector<Circle>& cs, const vector<Index>& order) {
        auto max_rad = MaxRadius(cs);

        Field field = BuildFieldFor(cs);
        for (auto i : order) {
            auto& c = cs[i];
            c.set_center(c.origin);
            if (field.HasIntersection(&c)) {
                Point best_center;
                auto best_sqr_distance = numeric_limits<double>::max();

                // need to go throgh each of two circles
                // no inner loop picking angle.
                for (auto j : order) {
                    if (i == j) break;

                    auto dist = c.radius + cs[j].radius + RADIUS_EPS;
                    for (auto phi = 0.; phi < 2*M_PI; phi += max_rad * 0.02) {
                        auto p = ant::geometry::d2::f::FromPolar({dist, phi}) + f::Indent(cs[j].center());
                        c.set_center(p);
                        if (!field.HasIntersection(&c) && best_sqr_distance > SqrDistance(p, c.origin)) {
                            best_sqr_distance = SqrDistance(p, c.origin);
                            best_center = p;
                        }
                    }
                }
                assert(best_sqr_distance != numeric_limits<double>::max());
                c.set_center(best_center);
            }
            field.Add(&c);
        }
    }
};