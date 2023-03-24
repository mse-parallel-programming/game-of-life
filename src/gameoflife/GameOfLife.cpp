//
// Created by kurbaniec on 24.03.2023.
//
#include <omp.h>
#include "GameOfLife.h"

namespace {

    int neighbourCount(int pos, int size, std::vector<Cell>& grid) {
        int count = 0;
        int upOffset = pos - size - 2;
        int downOffset = pos + size + 2;

        // Row about current one
        count += grid[upOffset-1] + grid[upOffset] + grid[upOffset+1];
        // Current row
        count += grid[pos-1] + grid[pos+1];
        // Row below current one
        count += grid[downOffset-1] + grid[downOffset] + grid[downOffset+1];

        return count;
    }

    void toBeOrNotToBe(int pos, int aliveNeighbours, std::vector<Cell>& grid, std::vector<Cell>& newGrid) {
        // https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#Rules
        if (grid[pos] == ALIVE) {
            // Any live cell with two or three live neighbours survives.
            if (aliveNeighbours == 2 || aliveNeighbours == 3)
                newGrid[pos] = ALIVE;
            // All other live cells die in the next generation.
            // Similarly, all other dead cells stay dead.
            // -- Note: no else is needed as DEAD is the default value
        } else if (aliveNeighbours == 3) {
            // Any dead cell with three live neighbours becomes a live cell.
            newGrid[pos] = ALIVE;
        }
    }

    void nextGeneration(int size, std::vector<Cell>& grid) {
        std::vector<Cell> newGrid(grid.size(), DEAD);

        #pragma omp parallel for \
        schedule(static) \
        default(none) firstprivate(size) shared(grid, newGrid)
        for (auto i = 0; i < size; ++i) {
            auto startIndex = size + 3 + (i * 2) + (i * size);
            for (auto j = 0; j < size; ++j) {
                auto pos = startIndex + j;
                auto aliveNeighbours = neighbourCount(pos, size, grid);
                toBeOrNotToBe(pos, aliveNeighbours, grid, newGrid);
            }
        }

        // Copy grid
        // https://stackoverflow.com/a/644677
        grid = newGrid;
    }

    void flattenAndPadGrid(
        int size,
        const std::vector<std::vector<Cell>>& grid,
        std::vector<Cell>& paddedGrid
    ) {
        for (auto i = 0; i < size; ++i) {
            auto startIndex = size + 3 + (i * 2) + (i * size);
            for (auto j = 0; j < size; ++j) {
                // auto pos = (i * size) + j;
                auto padPos = startIndex + j;
                paddedGrid[padPos] = grid[i][j];
            }
        }
    }
}

namespace GameOfLife {



    BenchmarkResult benchmark(
        const GameInput& input,
        const BenchmarkInput& benchmarkInput
    ) {
        auto size = input.size;
        auto& grid = input.grid;
        auto generations = benchmarkInput.generations;
        auto iterations = benchmarkInput.iterations;
        auto dynamic = benchmarkInput.dynamic;
        auto threadCount = benchmarkInput.threadCount;

        if (dynamic) omp_set_dynamic(*dynamic);
        if (threadCount) omp_set_num_threads(*threadCount);

        std::vector<std::chrono::duration<double, std::milli>> measurements;
        measurements.reserve(iterations);

        for (auto i = 0; i < iterations; ++i) {
            std::vector<Cell> paddedGrid((size+2)*(size+2), DEAD);
            flattenAndPadGrid(size, grid, paddedGrid);

            auto start = std::chrono::high_resolution_clock::now();
            for (auto g = 0; g < generations; ++g) {
                nextGeneration(size, paddedGrid);
            }
            auto end = std::chrono::high_resolution_clock::now();

            measurements.emplace_back(end-start);
        }


        return BenchmarkResult(measurements);
    }
}



