

static void SA_Benchmark(benchmark::State& state) {
    Index group = state.range(0);
    Problem pr = ProblemGroup::generate(group);

    auto cs = ProblemToCircles(pr);

    SA_v3<Rep_v3_Solver_New_2 < Score>> sa;
    Rep_v3_Solver_New_2 <Score> solver;
    solver.set_score(Score(1, 1));
    sa.set_solver(solver);
    sa.set_millis(10000);

    auto ps = sa.MinimumWork(pr);


    while (state.KeepRunning()) {
        bm.Remove(b, kMoveCount, kBeamWidth[group]);
    }
}

BENCHMARK(BeamSearchBenchmark)->DenseRange(0, kProblemGroupCount-1)->Unit(benchmark::kMillisecond)->Iterations(5)->ThreadPerCpu();




int main() {

}