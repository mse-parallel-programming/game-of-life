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

// class GameOfLife {
// public:
//     virtual void run(
//         int generations,
//         int size,
//         const std::vector<Cell>& grid,
//         std::function<void(std::vector<Cell>)> callback
//     );
//
//     virtual void benchmark(
//         int iterations,
//         int generations,
//         int size,
//         const std::vector<Cell>& grid
//     );
// };

namespace {
    void configureOpenMp(const std::optional<GameOfLife::ThreadConfig>& threadConfig);

    void nextGeneration(
        int size,
        std::vector<Cell>& oldGrid,
        std::vector<Cell>& newGrid
    );

    int neighbourCount(
        int pos, int size,
        std::vector<Cell>& grid
    );

    void toBeOrNotToBe(
        int pos,
        int aliveNeighbours,
        std::vector<Cell>& oldGrid,
        std::vector<Cell>& newGrid
    );

    void flattenAndPadGrid(
        int size,
        const std::vector<std::vector<Cell>>& grid,
        std::vector<Cell>& paddedGrid
    );

    void swapAndResetNewGrid(
        std::vector<Cell>& oldGrid,
        std::vector<Cell>& newGrid
    );
}

namespace GameOfLife {

    void run(
        const GameInput& input,
        const std::optional<ThreadConfig>& threadConfig,
        const std::function<bool(
            int generation, int size,
            std::vector<Cell>& oldGrid,
            std::vector<Cell>& newGrid
        )>& callback
    );

    BenchmarkResult benchmark(
        const GameInput& input,
        const BenchmarkInput& benchmarkInput
    );
};

#endif //PPR_GAME_OF_LIFE_GAMEOFLIFE_H
