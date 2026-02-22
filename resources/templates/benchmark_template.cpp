#include <benchmark/benchmark.h>
#include <vector>

// ============================================================
// Google Benchmark Template — CppAtlas
//
// Key APIs:
//   benchmark::DoNotOptimize(val)
//     Prevents the compiler from optimizing away the measured
//     expression.  Use around any value you compute inside the
//     loop.
//
//   benchmark::ClobberMemory()
//     Forces all pending writes to memory.  Use after writing
//     to a buffer to prevent the compiler from eliding the
//     stores.
//
//   state.range(0)
//     The parameter value when using ->Range() or ->Arg().
//
// Reference: https://github.com/google/benchmark
// ============================================================

// ── Simple function benchmark ─────────────────────────────────
static void BM_Example(benchmark::State& state) {
    for (auto _ : state) {
        int x = 42 * 42;
        benchmark::DoNotOptimize(x);
    }
}
BENCHMARK(BM_Example);

// ── Parametric benchmark (input size varies) ──────────────────
static void BM_VectorFill(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> v(static_cast<std::size_t>(state.range(0)));
        benchmark::DoNotOptimize(v.data());
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}
// Runs with N = 8, 16, 32, ... 8192
BENCHMARK(BM_VectorFill)->Range(8, 8 << 10)->Complexity();

BENCHMARK_MAIN();
