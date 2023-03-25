//
// Created by kurbaniec on 25.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_THREADCONFIG_H
#define PPR_GAME_OF_LIFE_THREADCONFIG_H

#include <optional>

namespace GameOfLife {
    struct ThreadConfig {
        bool dynamic;
        int threadCount;
    };
}

#endif //PPR_GAME_OF_LIFE_THREADCONFIG_H
