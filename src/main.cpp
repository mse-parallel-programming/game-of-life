#include <iostream>
#include <vector>
#include <algorithm>
#include "global.h"
#include "GameOfLife.h"
#include "sequential/GameOfLifeSeq.h"


int main() {

    // https://www.geeksforgeeks.org/program-for-conways-game-of-life/
    // int size = 10;
    // std::string textInput = ".........."
    //                         "...**....."
    //                         "....*....."
    //                         ".........."
    //                         ".........."
    //                         ".........."
    //                         ".........."
    //                         ".........."
    //                         ".........."
    //                         "..........";
    int size = 10;
    std::string textInput = ".........."
                            "...**....."
                            "....*....."
                            ".........."
                            ".........."
                            "...**....."
                            "..**......"
                            ".....*...."
                            "....*....."
                            "..........";
    // int size = 3;
    // std::string textInput = "..."
    //                         ".*."
    //                         "...";
    std::vector<char> rawInput(textInput.begin(), textInput.end());
    // Transform input
    // https://stackoverflow.com/a/51075101
    std::vector<Cell> input;
    input.reserve(rawInput.size());
    std::transform(rawInput.begin(), rawInput.end(),
                   std::back_inserter(input),
                   [](char elem) {
                       return elem == '.' ? DEAD : ALIVE;
                   });

    GameOfLifeSeq impl;
    GameOfLife& game = impl;

    auto printCallback = [size](const std::vector<Cell>& iterationGrid) {
        for (auto i = 0; i < size; ++i) {
            auto startIndex = size + 3 + (i * 2) + (i * size);
            for (auto j = 0; j < size; ++j) {
                auto pos = startIndex + j;
                auto cell = iterationGrid[pos];
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    };

    game.run(15, size, input, printCallback);
}
