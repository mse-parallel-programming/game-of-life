//
// Created by kurbaniec on 27.03.2023.
//

#include "Benchmark.h"
#include "../util/Input.h"
#include "../gameoflife/GameOfLife.h"

namespace Benchmark {
    void localBenchmark(
        int size,
        int generations,
        int iterations,
        const std::vector<int>& threads
    ) {
        auto input = Util::randomInput(size, 0.25);
        auto benchmarkInput = GameOfLife::BenchmarkInput {
            iterations, generations
        };

        // Warmup
        auto warmupIterations = std::max(5, iterations/10);
        auto warmupBenchmarkInput = GameOfLife::BenchmarkInput {
            warmupIterations, generations
        };
        std::cout << "Warmup" << std::endl;
        // Disable output
        // https://stackoverflow.com/a/30185095
        std::cout.setstate(std::ios_base::failbit);
        GameOfLife::benchmark(
            input, std::nullopt,
            warmupBenchmarkInput
        );
        std::cout.clear();
        std::cout << "Warmup completed" << std::endl << std::endl;

        // Benchmark
        // Optimal run
        GameOfLife::benchmark(
            input, std::nullopt,
            benchmarkInput
        );
        // Given thread counts
        for (auto threadCount : threads) {
            std::cout << std::endl;
            auto threadConfig = GameOfLife::ThreadConfig {
                false, threadCount
            };
            GameOfLife::benchmark(
                input, threadConfig,
                benchmarkInput
            );
        }
    }
}

