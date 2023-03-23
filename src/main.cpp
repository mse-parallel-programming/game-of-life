#include <iostream>
#include <vector>
#include <algorithm>
#include "global.h"
#include "GameOfLife.h"
#include "sequential/GameOfLifeSeq.h"
#include "util/input.h"


int main() {

    // https://www.geeksforgeeks.org/program-for-conways-game-of-life/
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
    // std::string textInput = "..."
    //                         ".*."
    //                         "...";

    auto input = Util::textInput(textInput);
    auto size = input.size;
    auto grid = input.grid;

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

    // game.run(15, size, grid, printCallback);
    game.benchmark(10, 1000, size, grid);
}
