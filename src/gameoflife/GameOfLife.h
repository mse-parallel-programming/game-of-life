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
#include "benchmark/BenchmarkResult.h"
#include "GameInput.h"
#include "benchmark/BenchmarkInput.h"

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
    void nextGeneration(
        int size,
        std::vector<Cell>& grid
    );

    int neighbourCount(
        int pos, int size,
        std::vector<Cell>& grid
    );

    void toBeOrNotToBe(
        int pos,
        int aliveNeighbours,
        std::vector<Cell>& grid,
        std::vector<Cell>& newGrid
    );

    void padGrid(
        int size,
        const std::vector<Cell>& grid,
        std::vector<Cell>& paddedGrid
    );
}

namespace GameOfLife {



    BenchmarkResult benchmark(
        const GameInput& input,
        const BenchmarkInput& benchmarkInput
    );
};

#endif //PPR_GAME_OF_LIFE_GAMEOFLIFE_H
