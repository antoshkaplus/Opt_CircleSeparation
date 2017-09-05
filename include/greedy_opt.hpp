#pragma once

#include <atomic>

#include "greedy.hpp"
#include "score.hpp"

// for each group we run mesh,
// with changing variants rad / mass
// 0.5, 2, 0.5 = step

static constexpr Count kSampleCount = 5;

// returns scores
template<class Score, class ScoreFactory>
inline vector<double> ComputeFactorMesh(Index group, ScoreFactory score_factory, string factor_title="factor", double min=0, double max=1, double step=0.1) {
    vector<Problem> problems(kSampleCount);
    for_each(problems.begin(), problems.end(), [&](Problem& p) {
        ProblemGroup p_group;
        p = p_group.generate(group);
    });

    auto func = [&](double factor) {

        auto res = accumulate(problems.begin(), problems.end(), 0., [&](double total, const Problem& pr) {
            Greedy<Score> solver;
            solver.set_score(score_factory(factor));

            auto ps = solver.MinimumWork(pr);

            return total +  pr.Work(ps);
        });
        return res;
    };

    vector<double> mesh;
    for (auto i = min; i <= max+step/2; i += step) mesh.push_back(i);
    vector<double> res(mesh.size(), 0);
    atomic_uint progress{0};
    tbb::parallel_for(0, (Index)mesh.size(), 1, [&](Index i) {
        res[i] = func(mesh[i]);
        ++progress;
        cout << "gr: " << group << " pr: " << progress << "/" << mesh.size() << endl;
    });
    return res;
}


inline void OptimizeRadFactorMesh() {
    vector<vector<double>> pows(ProblemGroup::count());

    tbb::parallel_for(0, ProblemGroup::count(), 1, [&](Index group) {

        try {
            auto score_factory = [](double rad_pow) {
                return Score(1, rad_pow);
            };

            pows[group] = ComputeFactorMesh<Score, decltype(score_factory)>(
                group, score_factory, "pows", 0., 0.1, 0.01);
        } catch (exception ex) {
            std::cout << "group: " << group << " " << ex.what() << endl;
        }
    });
    for (auto i = 0; i < ProblemGroup::count(); ++i) {
        Println(cout, pows[i], "mesh for group " + std::to_string(i));
    }
}