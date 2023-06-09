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
#include "../gameoflife/types/GameInput.h"

namespace Util {

    static GameOfLife::GameInput textInput(const std::string& textInput) {
        auto size = sqrt((double)textInput.size());
        // Check if integer
        // https://stackoverflow.com/a/1521682
        double intPart;
        if (modf(size, &intPart) != 0.0)
            throw std::runtime_error("Input width / height are not equal");
        auto intSize = (int)size;

        std::vector<char> rawInput(textInput.begin(), textInput.end());

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

    static std::string toText(const std::vector<std::vector<Cell>>& result) {
        auto size = result.size();
        std::string text;
        text.reserve(size);
        for (auto i = 0; i < size; ++i) {
            for (auto j = 0; j < size; ++j) {
                auto value = static_cast<bool>(result[i][j]);
                if (value) text.push_back('*');
                else text.push_back('.');
            }

        }
        return text;
    }

    static GameOfLife::GameInput randomInput(int size, double alivePercentage) {
        std::vector<std::vector<Cell>> input;
        input.reserve(size);

        for (auto i = 0; i < size; ++i) {
            std::vector<Cell> rowInput(size, DEAD);
            for (auto j = 0; j < size; j++) {
                // Random value with distribution
                auto val = (double)rand() / RAND_MAX;
                if (val < alivePercentage)
                    rowInput[i] = ALIVE;
            }
            input.emplace_back(rowInput);
        }

        return GameOfLife::GameInput { size, input };
    }
}

#endif //PPR_GAME_OF_LIFE_INPUT_H
