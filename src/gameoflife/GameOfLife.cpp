//
// Created by kurbaniec on 24.03.2023.
//
#include <omp.h>
#include <sstream>
#include <unordered_set>
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

    void toBeOrNotToBe(
        int pos,
        int aliveNeighbours,
        std::vector<Cell>& oldGrid,
        std::vector<Cell>& newGrid
    ) {
        // https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#Rules
        if (oldGrid[pos] == ALIVE) {
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

    void nextGeneration(
        int size,
        std::vector<Cell>& oldGrid,
        std::vector<Cell>& newGrid
    ) {
        // std::vector<Cell> newGrid(oldGrid.size(), DEAD);

        // https://stackoverflow.com/q/9953905

        // std::unordered_set<int> wow;

        // This could be it ...
        // https://stackoverflow.com/a/46115714

        #pragma omp parallel for collapse(1) \
        schedule(static) \
        default(none) firstprivate(size) shared(oldGrid, newGrid)
        for (auto i = 0; i < size; ++i) {
            auto startIndex = size + 3 + (i * 2) + (i * size);
            for (auto j = 0; j < size; ++j) {
                auto pos = startIndex + j;
                // #pragma omp critical
                // {
                //     if (wow.contains(pos)) {
                //         std::cout << pos << " arealdy used" << std::endl;
                //         throw std::runtime_error("hmm");
                //     }
                //     wow.emplace(pos);
                // };
                auto aliveNeighbours = neighbourCount(pos, size, oldGrid);
                toBeOrNotToBe(pos, aliveNeighbours, oldGrid, newGrid);
            }
        }
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

    void swap(std::vector<Cell>& oldGrid, std::vector<Cell>& newGrid)  {
        // Copy grid
        // https://stackoverflow.com/a/644677
        // oldGrid = newGrid;
        oldGrid.swap(newGrid);
        std::fill(newGrid.begin(), newGrid.end(), DEAD);

        auto sum = std::reduce(newGrid.begin(), newGrid.end()) / newGrid.size();
        std::cout << "sum: " << sum << std::endl;
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
            std::vector<Cell> oldGrid((size + 2) * (size + 2), DEAD);
            flattenAndPadGrid(size, grid, oldGrid);
            std::vector<Cell> newGrid((size + 2) * (size + 2), DEAD);

            auto start = std::chrono::high_resolution_clock::now();
            for (auto g = 0; g < generations; ++g) {
                nextGeneration(size, oldGrid, newGrid);
                swap(oldGrid, newGrid);
            }
            auto end = std::chrono::high_resolution_clock::now();

            measurements.emplace_back(end-start);
        }


        return BenchmarkResult(measurements);
    }

    void run(
        const GameInput& input,
        const std::function<bool(
            std::vector<Cell>& oldGrid,
            std::vector<Cell>& newGrid
        )>& callback
    ) {
        auto size = input.size;
        auto& grid = input.grid;

        std::vector<Cell> oldGrid((size + 2) * (size + 2), DEAD);
        flattenAndPadGrid(size, grid, oldGrid);
        std::vector<Cell> newGrid((size + 2) * (size + 2), DEAD);

        auto running = true;
        while(running) {
            nextGeneration(size, oldGrid, newGrid);
            running = callback(oldGrid, newGrid);

            std::stringstream out;
            for (auto i = 0; i < size; ++i) {
                auto startIndex = size + 3 + (i * 2) + (i * size);
                for (auto j = 0; j < size; ++j) {
                    auto pos = startIndex + j;
                    auto cell = newGrid[pos];
                    out << cell << " ";
                }
                out << " \n";
            }
            std::cout << out.str() << std::endl;

            swap(oldGrid, newGrid);
        }

    }
}



