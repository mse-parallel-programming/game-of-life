//
// Created by kurbaniec on 27.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_UPDATEACK_H
#define PPR_GAME_OF_LIFE_UPDATEACK_H

#include "json.hpp"

using json = nlohmann::json;

namespace Message {
    struct UpdateAck {
        bool next;
    };

    void from_json(const json&j, UpdateAck& msg);
}



#endif //PPR_GAME_OF_LIFE_UPDATEACK_H
