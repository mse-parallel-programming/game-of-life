//
// Created by kurbaniec on 24.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_STARTMESSAGE_H
#define PPR_GAME_OF_LIFE_STARTMESSAGE_H

#include "GameInput.h"
#include "BenchmarkInput.h"
#include "json.hpp"
using json = nlohmann::json;

namespace GameOfLife {
    struct StartMessage {
        GameOfLife::GameInput input;
        std::optional<GameOfLife::BenchmarkInput> benchmarkInput;
    };

    // https://json.nlohmann.me/features/arbitrary_types/
    // void to_json(json& j, const StartMessage& msg) {
    //     j = json{
    //         {"input", {
    //             {}
    //         }},
    //         {"benchmarkInput", msg.benchmarkInput}
    //     };
    // }

    void from_json(const json&j, StartMessage& msg) {
        auto input = j["input"];
        input.at("size").get_to(msg.input.size);
        input.at("grid").get_to(msg.input.grid);
        if (j.contains("benchmarkInput")) {
            msg.benchmarkInput = BenchmarkInput {};
            auto benchmarkInput = j["benchmarkInput"];
            benchmarkInput.at("iterations").get_to(msg.benchmarkInput->iterations);
            benchmarkInput.at("generations").get_to(msg.benchmarkInput->generations);
            if (benchmarkInput.contains("dynamic"))
                msg.benchmarkInput->dynamic = benchmarkInput["dynamic"].get<bool>();
            if (benchmarkInput.contains("threadCount"))
                msg.benchmarkInput->threadCount = benchmarkInput["threadCount"].get<int>();
        } else {
            msg.benchmarkInput = std::nullopt;
        }

    }
}


#endif //PPR_GAME_OF_LIFE_STARTMESSAGE_H
