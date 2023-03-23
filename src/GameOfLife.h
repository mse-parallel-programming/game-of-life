//
// Created by kurbaniec on 22.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_GAMEOFLIFE_H
#define PPR_GAME_OF_LIFE_GAMEOFLIFE_H

#include <vector>
#include <functional>
#include "global.h"

class GameOfLife {
public:
    virtual void run(
        int generations,
        int size,
        const std::vector<Cell>& grid,
        std::function<void(std::vector<Cell>)> callback
    );

    virtual void benchmark(
        int iterations,
        int generations,
        int size,
        const std::vector<Cell>& grid
    );
};

#endif //PPR_GAME_OF_LIFE_GAMEOFLIFE_H
