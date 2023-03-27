//
// Created by kurbaniec on 24.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_BENCHMARKRESULT_H
#define PPR_GAME_OF_LIFE_BENCHMARKRESULT_H

#include <vector>
#include <chrono>
#include <numeric>
#include <iostream>

namespace GameOfLife {
    struct BenchmarkResult {
    public:
        std::vector<std::chrono::duration<double, std::milli>> measurements;
        std::chrono::duration<double, std::milli> averageTime;
        std::chrono::duration<double, std::milli> minTime;
        std::chrono::duration<double, std::milli> maxTime;

        explicit BenchmarkResult(
            std::vector<std::chrono::duration<double, std::milli>> measurements
        ) : averageTime(std::reduce(measurements.begin(), measurements.end()) / measurements.size()),
            // https://stackoverflow.com/a/28574413
            minTime(*std::min_element(measurements.begin(), measurements.end())),
            maxTime( *std::max_element(measurements.begin(), measurements.end())) {}

        void print() const {
            std::cout << "Benchmark result" << std::endl;
            std::cout << "  Average time: " << averageTime.count() << " ms" << std::endl;
            std::cout << "  Min time: " << minTime.count() << " ms" << std::endl;
            std::cout << "  Max time: " << maxTime.count() << " ms" << std::endl;
        }
    };
}


#endif //PPR_GAME_OF_LIFE_BENCHMARKRESULT_H
