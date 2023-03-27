//
// Created by kurbaniec on 27.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_BENCHMARK_H
#define PPR_GAME_OF_LIFE_BENCHMARK_H

#include <vector>
#include "json.hpp"
#include "../gameoflife/types/BenchmarkResult.h"
#include "../global.h"

using json = nlohmann::json;

namespace Message {
    struct Benchmark {
        std::vector<std::vector<bool>> grid;
        double averageTime;
        double minTime;
        double maxTime;

        explicit Benchmark(
            const std::tuple<
                GameOfLife::BenchmarkResult,
                std::vector<std::vector<Cell>>
            >& result
        ) :
            averageTime(std::get<0>(result).averageTime.count()),
            minTime(std::get<0>(result).minTime.count()),
            maxTime(std::get<0>(result).maxTime.count()) {
            auto resultGrid = std::get<1>(result);
            // https://stackoverflow.com/a/30735055
            grid.reserve(resultGrid.size());
            for (auto&& row: resultGrid)
                grid.emplace_back(std::begin(row), std::end(row));
        }
    };

    void to_json(json& j, const Benchmark& msg);
}


#endif //PPR_GAME_OF_LIFE_BENCHMARK_H
