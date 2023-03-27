//
// Created by kurbaniec on 27.03.2023.
//

#include "Update.h"

namespace Message {
    void to_json(json& j, const Update& msg) {
        // https://json.nlohmann.me/features/arbitrary_types/
        j = json{
            {"generation", msg.generation},
            {"diff",       msg.diff}
        };
    };
}