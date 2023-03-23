//
// Created by kurbaniec on 22.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_GAMEOFLIFESEQ_H
#define PPR_GAME_OF_LIFE_GAMEOFLIFESEQ_H


#include "../GameOfLife.h"
// #include "../global.h"
// #include <vector>
// #include <functional>

class GameOfLifeSeq : public GameOfLife {
public:
    void run(
        int generations,
        int size,
        const std::vector<Cell>& grid,
        std::function<void(std::vector<Cell>)> callback
    ) override;

    void benchmark(
        int iterations,
        int generations,
        int size,
        const std::vector<Cell>& grid
    ) override;
private:
    int neighbourCount(
        int pos, int size,
        std::vector<Cell>& grid
    ) const;

    void toBeOrNotToBe(
        int pos,
        int aliveNeighbours,
        std::vector<Cell>& grid,
        std::vector<Cell>& newGrid
    );

    void nextGeneration(
        int size,
        std::vector<Cell>& grid
    );

    void padGrid(
        int size,
        const std::vector<Cell>& grid,
        std::vector<Cell>& paddedGrid
    );
};


#endif //PPR_GAME_OF_LIFE_GAMEOFLIFESEQ_H
