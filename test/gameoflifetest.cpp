//
// Created by kurbaniec on 04.04.2023.
//

#include <gtest/gtest.h>
#include "../src/gameoflife/GameOfLife.h"
#include "../src/util/Input.h"



// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);

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

    auto input = Util::textInput(textInput);
    auto benchmarkInput = GameOfLife::BenchmarkInput {1, 1};

    auto result = GameOfLife::benchmarkWithGrid(input, std::nullopt, benchmarkInput);
    auto actual = Util::toText(std::get<1>(result));

    std::string expected = ".........."
                         "...**....."
                         "...**....."
                         ".........."
                         ".........."
                         "..***....."
                         "..**......"
                         "...**....."
                         ".........."
                         "..........";

    EXPECT_EQ(actual, expected);
}

TEST(HelloTest2, BasicAssertions) {
    std::string textInput = ".........."
                            ".........."
                            ".........."
                            ".........."
                            ".....*...."
                            "......*..."
                            "....***..."
                            ".........."
                            ".........."
                            "..........";

    auto input = Util::textInput(textInput);
    auto benchmarkInput = GameOfLife::BenchmarkInput {1, 1};

    auto result = GameOfLife::benchmarkWithGrid(input, std::nullopt, benchmarkInput);
    auto actual = Util::toText(std::get<1>(result));

    std::string expected = ".........."
                           ".........."
                           ".........."
                           ".........."
                           ".........."
                           "....*.*..."
                           ".....**..."
                           ".....*...."
                           ".........."
                           "..........";

    EXPECT_EQ(actual, expected);

}

TEST(HelloTest3, BasicAssertions) {
    std::string textInput = ".........."
                            ".........."
                            ".........."
                            ".........."
                            ".....*...."
                            "......*..."
                            "....***..."
                            ".........."
                            ".........."
                            "..........";

    auto input = Util::textInput(textInput);
    auto benchmarkInput = GameOfLife::BenchmarkInput {1, 2};

    auto result = GameOfLife::benchmarkWithGrid(input, std::nullopt, benchmarkInput);
    auto actual = Util::toText(std::get<1>(result));

    std::string expected = ".........."
                           ".........."
                           ".........."
                           ".........."
                           ".........."
                           "......*..."
                           "....*.*..."
                           ".....**..."
                           ".........."
                           "..........";

    EXPECT_EQ(actual, expected);

}
