//
// Created by kurbaniec on 27.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_BENCHMARK_H
#define PPR_GAME_OF_LIFE_BENCHMARK_H


#include "../gameoflife/types/BenchmarkResult.h"

namespace Benchmark {

    void localBenchmark(
        int iterations,
        int generations,
        int size,
        const std::vector<int>& threads
    );

    GameOfLife::BenchmarkResult remoteBenchmark(

    );
}



#endif //PPR_GAME_OF_LIFE_BENCHMARK_H