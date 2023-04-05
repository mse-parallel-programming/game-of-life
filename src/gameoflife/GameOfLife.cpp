//
// Created by kurbaniec on 24.03.2023.
//
#include <omp.h>
#include <sstream>
#include <unordered_set>
#include <thread>
#include "GameOfLife.h"

namespace {

    void configureOpenMp(
        const std::optional<GameOfLife::ThreadConfig>& threadConfig
    ) {
        bool dynamic;
        int threadCount;

        if (threadConfig) {
            dynamic = threadConfig->dynamic;
            threadCount = threadConfig->threadCount;
        } else {
            dynamic = true;
            threadCount = (int) std::thread::hardware_concurrency();
        }

        // Configure openmp to used fixed thread size
        // https://stackoverflow.com/a/11096742
        omp_set_dynamic(dynamic);
        omp_set_num_threads(threadCount);

        std::cout << "  OpenMP Dynamic: " << dynamic << std::endl;
        std::cout << "  OpenMP Thread Count: " << threadCount << std::endl;
    }

    int neighbourCount(int pos, int size, const CellA* grid) {
        int count = 0;
        auto upOffset = pos - size - 2 - falseSharingPadding;
        auto downOffset = pos + size + 2 + falseSharingPadding;

        // Row about current one
        count += grid[upOffset - 1] + grid[upOffset] + grid[upOffset + 1];
        // Current row
        count += grid[pos - 1] + grid[pos + 1];
        // Row below current one
        count += grid[downOffset - 1] + grid[downOffset] + grid[downOffset + 1];

        return count;
    }

    void toBeOrNotToBe(int pos, int aliveNeighbours, const CellA* oldGrid, CellA* newGrid) {
        // https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#Rules
        if (oldGrid[pos] == ALIVE) {
            // Any live cell with two or three live neighbours survives.
            // All other live cells die in the next generation.
            if (aliveNeighbours == 2 || aliveNeighbours == 3) newGrid[pos] = ALIVE;
            else newGrid[pos] = DEAD;
        } else {
            // Any dead cell with three live neighbours becomes a live cell.
            // Similarly, all other dead cells stay dead.
            if (aliveNeighbours == 3) newGrid[pos] = ALIVE;
            else newGrid[pos] = DEAD;
        }
    }

    void nextGeneration(int size, CellA* oldGrid, CellA* newGrid) {
        #pragma omp parallel for collapse(1) \
        schedule(static) \
        default(none) firstprivate(size, oldGrid, newGrid)
        for (auto i = 0; i < size; ++i) {
            auto startIndex = GameOfLife::rowStartIndexAt(i, size);
            for (auto j = 0; j < size; ++j) {
                auto pos = startIndex + j;
                auto aliveNeighbours = neighbourCount(pos, size, oldGrid);
                toBeOrNotToBe(pos, aliveNeighbours, oldGrid, newGrid);
            }
        }
    }

    void flattenAndPadGrid(int size, const std::vector<std::vector<Cell>>& grid, CellA* paddedGrid) {
        for (auto i = 0; i < size; ++i) {
            auto startIndex = GameOfLife::rowStartIndexAt(i, size);
            for (auto j = 0; j < size; ++j) {
                auto padPos = startIndex + j;
                paddedGrid[padPos] = static_cast<CellA>(grid[i][j]);
            }
        }
    }

    std::vector<std::vector<Cell>> gridToVec(int size, const CellA* grid) {
        std::vector<std::vector<Cell>> gridResult;
        gridResult.reserve(size);
        for (auto i = 0; i < size; ++i) {
            auto startIndex = GameOfLife::rowStartIndexAt(i, size);
            std::vector<Cell> row;
            row.reserve(size);
            for (auto j = 0; j < size; ++j) {
                auto pos = startIndex + j;
                row.emplace_back(static_cast<Cell>(grid[pos]));
            }
            gridResult.emplace_back(row);
        }
        return gridResult;
    }

    void swapAndResetNewGrid(std::vector<Cell>& oldGrid, std::vector<Cell>& newGrid) {
        oldGrid.swap(newGrid);
        std::fill(newGrid.begin(), newGrid.end(), DEAD);
    }


}

namespace GameOfLife {

    void run(
        const GameInput& input,
        const std::optional<ThreadConfig>& threadConfig,
        const std::function<bool(
            int generation, int size,
            const CellA* oldGrid, const CellA* newGrid
        )>& callback
    ) {
        std::cout << "Game of Life" << std::endl;

        auto size = input.size;
        auto& grid = input.grid;
        std::cout << "Parameters:  " << std::endl;
        std::cout << "  Size: " << size << std::endl;

        configureOpenMp(threadConfig);

        auto paddedSize = (size + 2 + falseSharingPadding) * (size + 2);
        auto* oldGrid = new CellA[paddedSize];
        auto* newGrid = new CellA[paddedSize];
        std::fill_n(oldGrid, paddedSize, DEAD);
        flattenAndPadGrid(size, grid, oldGrid);
        std::fill_n(newGrid, paddedSize, DEAD);


        auto running = true;
        auto generation = 0;
        while (running) {
            ++generation;
            nextGeneration(size, oldGrid, newGrid);
            running = callback(generation, size, oldGrid, newGrid);
            std::swap(oldGrid, newGrid);
        }

    }

    BenchmarkResult benchmark(
        const GameInput& input,
        const std::optional<ThreadConfig>& threadConfig,
        const BenchmarkInput& benchmarkInput
    ) {
        auto result = benchmarkWithGrid(input, threadConfig, benchmarkInput);
        return std::get<0>(result);
    }

    std::tuple<BenchmarkResult, std::vector<std::vector<Cell>>>
    benchmarkWithGrid(
        const GameInput& input,
        const std::optional<ThreadConfig>& threadConfig,
        const BenchmarkInput& benchmarkInput
    ) {
        std::cout << "Game of Life" << std::endl;

        auto size = input.size;
        auto& grid = input.grid;
        auto generations = benchmarkInput.generations;
        auto iterations = benchmarkInput.iterations;
        std::cout << "Parameters:  " << std::endl;
        std::cout << "  Size: " << size << std::endl;
        std::cout << "  Generations: " << generations << std::endl;
        std::cout << "  Iterations: " << iterations << std::endl;

        configureOpenMp(threadConfig);

        std::vector<std::chrono::duration<double, std::milli>> measurements;
        measurements.reserve(iterations);

        auto paddedSize = (size + 2 + falseSharingPadding) * (size + 2);
        auto* oldGrid = new CellA[paddedSize];
        auto* newGrid = new CellA[paddedSize];

        for (auto i = 0; i < iterations; ++i) {
            std::fill_n(oldGrid, paddedSize, DEAD);
            flattenAndPadGrid(size, grid, oldGrid);
            std::fill_n(newGrid, paddedSize, DEAD);

            auto start = std::chrono::high_resolution_clock::now();

            for (auto g = 0; g < generations; ++g) {
                nextGeneration(size, oldGrid, newGrid);
                std::swap(oldGrid, newGrid);
            }

            auto end = std::chrono::high_resolution_clock::now();
            measurements.emplace_back(end - start);
        }

        auto benchmarkResult = BenchmarkResult(measurements);
        benchmarkResult.print();

        auto gridResult = gridToVec(size, oldGrid);

        delete[] newGrid;
        delete[] oldGrid;

        return {benchmarkResult, gridResult};
    }

    int rowStartIndexAt(int i, int size) {
        // Returns the index equivalent to the i_th row of a double dimensional grid
        // Is a bit complicated as the used grid is an one dimensional array, padded with dead cells
        // and additional bytes to mitigate false sharing.
        // --------------------------------------------------------
        // Returning std::size_t resulting here to performance loss
        // `hardware_destructive_interference_size` casted to int (`falseSharingPadding)` resolves this issue.
        return size + 3 + falseSharingPadding + (i * 2) + (i * falseSharingPadding) + (i * size);
    }
}



