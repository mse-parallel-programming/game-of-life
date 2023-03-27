//
// Created by kurbaniec on 27.03.2023.
//

#include "UpdateAck.h"

namespace Message {

    void from_json(const json& j, UpdateAck& msg) {
        j.at("next").get_to(msg.next);
    }
}

