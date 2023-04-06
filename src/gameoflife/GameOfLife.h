//
// Created by kurbaniec on 22.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_GAMEOFLIFE_H
#define PPR_GAME_OF_LIFE_GAMEOFLIFE_H

#include <utility>
#include <vector>
#include <functional>
#include <chrono>

#include "../global.h"
#include "types/BenchmarkResult.h"
#include "types/GameInput.h"
#include "types/BenchmarkInput.h"
#include "types/ThreadConfig.h"

namespace {
    void configureOpenMp(const std::optional<GameOfLife::ThreadConfig>& threadConfig);

    void nextGeneration(
        int size,
        Cell* oldGrid,
        Cell* newGrid
    );

    int neighbourCount(
        int pos, int size,
        const Cell* grid
    );

    void toBeOrNotToBe(
        int pos,
        int aliveNeighbours,
        const Cell* oldGrid,
        Cell* newGrid
    );

    void flattenAndPadGrid(
        int size,
        const std::vector<std::vector<Cell>>& grid,
        Cell* paddedGrid
    );

    std::vector<std::vector<Cell>> gridToVec(int size, const Cell* grid);
}

namespace GameOfLife {

    void run(
        const GameInput& input,
        const std::optional<ThreadConfig>& threadConfig,
        const std::function<bool(
            int generation, int size,
            const Cell* oldGrid, const Cell* newGrid
        )>& callback
    );

    BenchmarkResult benchmark(
        const GameInput& input,
        const std::optional<ThreadConfig>& threadConfig,
        const BenchmarkInput& benchmarkInput
    );

    std::tuple<BenchmarkResult, std::vector<std::vector<Cell>>> benchmarkWithGrid(
        const GameInput& input,
        const std::optional<ThreadConfig>& threadConfig,
        const BenchmarkInput& benchmarkInput
    );

    int rowStartIndexAt(int i, int size);
};

#endif //PPR_GAME_OF_LIFE_GAMEOFLIFE_H
