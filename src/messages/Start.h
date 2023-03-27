//
// Created by kurbaniec on 24.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_START_H
#define PPR_GAME_OF_LIFE_START_H

#include "../gameoflife/types/GameInput.h"
#include "../gameoflife/types/BenchmarkInput.h"
#include "json.hpp"
#include "../gameoflife/types/ThreadConfig.h"

using json = nlohmann::json;

namespace Message {
    struct Start {
        GameOfLife::GameInput input;
        std::optional<GameOfLife::ThreadConfig> threadConfig;
        std::optional<GameOfLife::BenchmarkInput> benchmarkInput;
    };


    // void to_json(json& j, const StartMessage& msg) {
    //     j = json{
    //         {"input", {
    //             {}
    //         }},
    //         {"benchmarkInput", msg.benchmarkInput}
    //     };
    // }

    void from_json(const json&j, Start& msg);
}


#endif //PPR_GAME_OF_LIFE_START_H
