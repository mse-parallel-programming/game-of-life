//
// Created by kurbaniec on 27.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_PARSE_H
#define PPR_GAME_OF_LIFE_PARSE_H

#include <vector>
#include <string>
#include <sstream>

namespace Util {
    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> values;
        std::stringstream stream(str);
        std::string tmp;
        while (std::getline(stream, tmp, delimiter))
            values.push_back(tmp);
        return values;
    }

    std::vector<int> strToInt(const std::vector<std::string>& strings) {
        std::vector<int> values;
        values.reserve(strings.size());
        for (auto& str: strings) {
            values.push_back(std::stoi(str));
        }
        return values;
    }


}

#endif //PPR_GAME_OF_LIFE_PARSE_H
