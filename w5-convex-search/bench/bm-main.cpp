#include <benchmark/benchmark.h>

void somefunction(int &x) {
    x = 0;
    return;
}

static void bm_somefunction(benchmark::State &state) {
    // set up the benchmark
    int x;

    for (auto _ : state) {
        // This code gets benchmarked
        somefunction(x);
    }
}

BENCHMARK(bm_somefunction);

BENCHMARK_MAIN();
