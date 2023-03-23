//
// Created by kurbaniec on 22.03.2023.
//

#include <iostream>
#include "GameOfLifeSeq.h"
#include <chrono>

int neighbourCount(
    int pos, int size,
    std::vector<Cell>& grid
) {
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
    std::vector<Cell>& grid,
    std::vector<Cell>& newGrid
) {
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

void nextGeneration(
    int size,
    std::vector<Cell>& grid
) {
    std::vector<Cell> newGrid(grid.size(), DEAD);

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

void padGrid(
    int size,
    const std::vector<Cell>& grid,
    std::vector<Cell>& paddedGrid
) {
    for (auto i = 0; i < size; ++i) {
        auto startIndex = size + 3 + (i * 2) + (i * size);
        for (auto j = 0; j < size; ++j) {
            auto pos = (i * size) + j;
            auto padPos = startIndex + j;
            paddedGrid[padPos] = grid[pos];
        }
    }
}

void GameOfLifeSeq::run(
    int generations,
    int size,
    const std::vector<Cell>& grid,
    std::function<void(std::vector<Cell>)> callback
) const {
    std::vector<Cell> paddedGrid((size+2)*(size+2), DEAD);
    padGrid(size, grid, paddedGrid);

    for (auto g = 0; g < generations; ++g) {
        nextGeneration(size, paddedGrid);
        callback(paddedGrid);
    }

}

void GameOfLifeSeq::benchmark(
    int iterations,
    int generations,
    int size,
    const std::vector<Cell>& grid
) const {

    std::chrono::duration<double, std::milli> averageTime
        = std::chrono::milliseconds::zero();

    for (auto i = 0; i < iterations; ++i) {
        std::vector<Cell> paddedGrid((size+2)*(size+2), DEAD);
        padGrid(size, grid, paddedGrid);

        auto start = std::chrono::high_resolution_clock::now();
        for (auto g = 0; g < generations; ++g) {
            nextGeneration(size, paddedGrid);
        }
        auto end = std::chrono::high_resolution_clock::now();

        averageTime += end-start;
    }

    averageTime /= iterations;

    std::cout << "Average time: " << averageTime.count() << " ms";
}

