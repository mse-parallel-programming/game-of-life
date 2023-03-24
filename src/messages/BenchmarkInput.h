//
// Created by kurbaniec on 24.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_BENCHMARKINPUT_H
#define PPR_GAME_OF_LIFE_BENCHMARKINPUT_H


#include <optional>

namespace GameOfLife {
    struct BenchmarkInput {
        int iterations;
        int generations;
        std::optional<bool> dynamic;
        std::optional<int> threadCount;
    };
}



#endif //PPR_GAME_OF_LIFE_BENCHMARKINPUT_H
