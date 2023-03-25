//
// Created by kurbaniec on 25.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_UPDATEMESSAGE_H
#define PPR_GAME_OF_LIFE_UPDATEMESSAGE_H

#include <map>
#include <vector>
#include "../global.h"
#include "json.hpp"

using json = nlohmann::json;

namespace GameOfLife {
    struct UpdateMessage {
        int generation;
        std::map<int, std::vector<int>> diff;

        static UpdateMessage from(
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
            return UpdateMessage{generation, diff};
        }
    };

    void to_json(json& j, const UpdateMessage& msg) {
        j = json{
            {"generation", msg.generation},
            {"diff",       msg.diff}
        };
    }
}

#endif //PPR_GAME_OF_LIFE_UPDATEMESSAGE_H
