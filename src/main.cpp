#include <iostream>
#include <vector>
#include <algorithm>
#include "global.h"
#include "GameOfLife.h"
#include "sequential/GameOfLifeSeq.h"




int main() {

    int size = 10;
    std::string textInput = ".........."
                            "...**....."
                            "....*....."
                            ".........."
                            ".........."
                            ".........."
                            ".........."
                            ".........."
                            ".........."
                            "..........";
    std::vector<char> rawInput(textInput.begin(), textInput.end());
    // Transform input
    // https://stackoverflow.com/a/51075101
    std::vector<Cell> input(rawInput.size());
    std::transform(rawInput.begin(), rawInput.end(),
                   std::back_inserter(input),
                   [](char elem) {
                       return elem == '.' ? DEAD : ALIVE;
                   });

    GameOfLifeSeq impl;
    GameOfLife &game = impl;
    game.run(size, input, [size](const std::vector<Cell>& iteration) {
        for (auto i = 0; i < size; ++i) {
            auto startIndex = size + 3 + (i * 2);
            for (auto j = 0; j < size; ++j) {
                auto pos = startIndex + j;
                std::cout << " " << pos;
            }
            std::cout << std::endl;
        }
    });
}
