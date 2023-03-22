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
        int size,
        std::vector<Cell> grid,
        std::function<void(std::vector<Cell>)> callback
    ) const override;
};


#endif //PPR_GAME_OF_LIFE_GAMEOFLIFESEQ_H