//
// Created by kurbaniec on 24.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_GAMEINPUT_H
#define PPR_GAME_OF_LIFE_GAMEINPUT_H

#include <vector>
#include "../global.h"

namespace GameOfLife {
    struct GameInput {
        int size;
        std::vector<Cell> grid;
    };
}



#endif //PPR_GAME_OF_LIFE_GAMEINPUT_H
