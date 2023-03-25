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
    // struct GameInput {
    // public:
    //     int size;
    //     std::vector<Cell> grid;
    // };

    static GameOfLife::GameInput textInput(const std::string& textInput) {
        auto size = sqrt((double)textInput.size());
        // Check if integer
        // https://stackoverflow.com/a/1521682
        double intPart;
        if (modf(size, &intPart) != 0.0)
            throw std::runtime_error("Input width / height are not equal");
        auto intSize = (int)size;

        std::vector<char> rawInput(textInput.begin(), textInput.end());


        /*// Transform input
        // https://stackoverflow.com/a/51075101
        std::vector<Cell> input;
        input.reserve(rawInput.size());
        std::transform(
            rawInput.begin(), rawInput.end(),
            std::back_inserter(input),
            [](char elem) {
                return elem == '.' ? DEAD : ALIVE;
            });

        return GameInput {(int) size, input };*/

        std::vector<std::vector<Cell>> input;
        input.reserve(intSize);
        for (auto i = 0; i < intSize; ++i) {
            std::vector<Cell> rowInput;
            input.reserve(intSize);
            for (auto j = 0; j < intSize; ++j) {
                auto elem = rawInput[(i * intSize) + j];
                if (elem == '.')
                    rowInput.emplace_back(DEAD);
                else
                    rowInput.emplace_back(ALIVE);
            }
            input.emplace_back(rowInput);
        }

        return GameOfLife::GameInput { intSize, input };
    }

    /*static GameInput randomInput(int size, double alivePercentage) {
        int length = size*size;
        std::vector<Cell> input(length, DEAD);

        for (auto i = 0; i < length; ++i) {
            // Random value with distribution
            auto val = (double)rand() / RAND_MAX;
            if (val < alivePercentage)
                input[i] = ALIVE;
        }

        return GameInput { size, input };
    }*/
}

#endif //PPR_GAME_OF_LIFE_INPUT_H
