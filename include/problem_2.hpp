#pragma once


struct Problem {
    Problem(const vector<::Circle>& circles) {
        circles_ = circles;
        auto max_r = max_element(circles.begin(), circles.end(), [](const ::Circle& c_0, const ::Circle& c_1) {
            return c_0.radius < c_1.radius;
        })->radius;
        field.Reset({-3, -3}, {4, 4}, {2*max_r, 2*max_r});
        field.Add(circles_.data(), circles_.data() + circles_.size());
    }

    ::Circle& operator[](Index i) {
        return circles_[i];
    }
    const ::Circle& operator[](Index i) const {
        return circles_[i];
    }

    auto begin() {
        return circles_.begin();
    }
    auto begin() const {
        return circles_.begin();
    }

    auto end() {
        return circles_.end();
    }
    auto end() const {
        return circles_.end();
    }

    Count Size() const {
        return circles_.size();
    }

    // if -1 then no overlaps at all
    //Index BiggestOverlap()

    // sorting from biggest to smallest
    template<class Score>
    void Order(vector<Index>& order, Score& score) {

    }

    bool HasOverlap(Index i) {
        return field.HasIntersection(&circles_[i]);
    }


    void Swap(Index i_0, Index i_1) {
        Point p_0 = circles_[i_0].center();
        Point p_1 = circles_[i_1].center();
        field.Relocate(&circles_[i_0], p_1);
        field.Relocate(&circles_[i_1], p_0);
    }


    double WorkChange(Index i, const Point& new_center) const {
        auto& c = circles_[i];
        auto& o = c.origin;
        return c.mass * (o.Distance(new_center) - o.Distance(c.center()));
    }

    double Work() const {
        double work = 0;
        for (auto& c : circles_) {
            work += c.mass * c.origin.Distance(c.center());
        }
        return work;
    }

public:
    Field field;
private:
    vector<::Circle> circles_;
};

// goal is to minimize work
struct ProblemState {
    ProblemState(const Problem& problem) {
        centers.reserve(problem.Size());
        work = problem.Work();
        for (auto& c : problem) {
            centers.push_back(c.center());
        }
    }

    void Bring(Problem& problem) {
        auto N = centers.size();
        for (int i = 0; i < N; ++i) {
            problem.field.Relocate(&problem[i], centers[i]);
        }
    }

    vector<Point> centers;
    double work;
};
