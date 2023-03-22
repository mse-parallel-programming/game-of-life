//
// Created by kurbaniec on 22.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_INPUT_H
#define PPR_GAME_OF_LIFE_INPUT_H

#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include "../global.h"

namespace Util {
    struct TextInput {
    public:
        int size;
        std::vector<Cell> grid;
    };

    static TextInput textInput(const std::string& textInput) {
        auto size = sqrt((double)textInput.size());
        // Check if integer
        // https://stackoverflow.com/a/1521682
        double intPart;
        if (modf(size, &intPart) != 0.0)
            throw std::runtime_error("Input width / height are not equal");

        std::vector<char> rawInput(textInput.begin(), textInput.end());
        // Transform input
        // https://stackoverflow.com/a/51075101
        std::vector<Cell> input;
        input.reserve(rawInput.size());
        std::transform(
            rawInput.begin(), rawInput.end(),
            std::back_inserter(input),
            [](char elem) {
                return elem == '.' ? DEAD : ALIVE;
            });

        return TextInput {(int) size, input };
    }
}

#endif //PPR_GAME_OF_LIFE_INPUT_H
