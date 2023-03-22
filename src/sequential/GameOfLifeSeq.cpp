//
// Created by kurbaniec on 22.03.2023.
//

#include "GameOfLifeSeq.h"

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

void nextGeneration(
    int size,
    std::vector<Cell>& grid
) {
    std::vector<Cell> newGrid(2);

    for (auto i = 0; i < size; ++i) {
        auto startIndex = size + 3 + (i * 2);
        for (auto j = 0; j < size; ++j) {
            auto pos = startIndex + j;
            auto aliveNeighbours = neighbourCount(pos, size, grid);
        }
    }

    grid = newGrid;
}

void GameOfLifeSeq::run(
    int size,
    const std::vector<Cell> grid,
    std::function<void(std::vector<Cell>)> callback
) const {



    callback(grid);
}

