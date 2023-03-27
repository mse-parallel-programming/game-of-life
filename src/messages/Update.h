//
// Created by kurbaniec on 25.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_UPDATE_H
#define PPR_GAME_OF_LIFE_UPDATE_H

#include <map>
#include <vector>
#include "../global.h"
#include "json.hpp"

using json = nlohmann::json;

namespace Message {
    struct Update {
        int generation;
        std::map<int, std::vector<int>> diff;

        static Update from(
            int generation,
            int size,
            const std::vector<Cell>& oldGrid,
            const std::vector<Cell>& newGrid
        ) {
            std::map<int, std::vector<int>> diff;
            for (auto i = 0; i < size; ++i) {
                auto startIndex = size + 3 + (i * 2) + (i * size);
                std::vector<int> rowDiff;
                for (auto j = 0; j < size; ++j) {
                    auto pos = startIndex + j;
                    if (oldGrid[pos] != newGrid[pos])
                        rowDiff.emplace_back(j);
                }
                if (!rowDiff.empty())
                    diff[i] = rowDiff;
            }
            return Update{generation, diff};
        }
    };

    void to_json(json& j, const Update& msg);
}

#endif //PPR_GAME_OF_LIFE_UPDATE_H
