//
// Created by kurbaniec on 27.03.2023.
//

#include "Benchmark.h"

namespace Message {

    void to_json(json& j, const Benchmark& msg) {
        // https://json.nlohmann.me/features/arbitrary_types/
        j = json{
            {"grid", msg.grid},
            {"averageTime", msg.averageTime},
            {"minTime", msg.minTime},
            {"maxTime", msg.maxTime},
        };
    }
}