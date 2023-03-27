//
// Created by kurbaniec on 27.03.2023.
//

#include "Start.h"

namespace Message {
    void from_json(const json&j, Start& msg) {
        // https://json.nlohmann.me/features/arbitrary_types/
        auto input = j["input"];
        input.at("size").get_to(msg.input.size);
        input.at("grid").get_to(msg.input.grid);

        if (j.contains("threadConfig")) {
            msg.threadConfig = GameOfLife::ThreadConfig {};
            auto threadConfig = j["threadConfig"];
            threadConfig.at("dynamic").get_to(msg.threadConfig->dynamic);
            threadConfig.at("threadCount").get_to(msg.threadConfig->threadCount);
        }

        if (j.contains("benchmarkInput")) {
            msg.benchmarkInput = GameOfLife::BenchmarkInput {};
            auto benchmarkInput = j["benchmarkInput"];
            benchmarkInput.at("iterations").get_to(msg.benchmarkInput->iterations);
            benchmarkInput.at("generations").get_to(msg.benchmarkInput->generations);
            // if (benchmarkInput.contains("dynamic"))
            //     msg.benchmarkInput->dynamic = benchmarkInput["dynamic"].get<bool>();
            // if (benchmarkInput.contains("threadCount"))
            //     msg.benchmarkInput->threadCount = benchmarkInput["threadCount"].get<int>();
        } else {
            msg.benchmarkInput = std::nullopt;
        }
    }
}