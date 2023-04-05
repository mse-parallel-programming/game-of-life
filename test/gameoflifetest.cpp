//
// Created by kurbaniec on 04.04.2023.
//

#include <gtest/gtest.h>
#include "../src/gameoflife/GameOfLife.h"
#include "../src/util/Input.h"

std::string gameOfLife(const std::string& textInput, int generations) {
    auto input = Util::textInput(textInput);
    auto benchmarkInput = GameOfLife::BenchmarkInput{1, generations};

    auto result = GameOfLife::benchmarkWithGrid(input, std::nullopt, benchmarkInput);
    return Util::toText(std::get<1>(result));
}

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

TEST(GameOfLife, Generation1) {
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
    auto actual = gameOfLife(textInput, 1);
    EXPECT_EQ(actual, expected);
}

TEST(GameOfLife, Generation2) {
    std::string expected = ".........."
                           "...**....."
                           "...**....."
                           ".........."
                           "...*......"
                           "..*.*....."
                           ".........."
                           "..***....."
                           ".........."
                           "..........";
    auto actual = gameOfLife(textInput, 2);
    EXPECT_EQ(actual, expected);
}

TEST(GameOfLife, Generation3) {
    std::string expected = ".........."
                           "...**....."
                           "...**....."
                           "...**....."
                           "...*......"
                           "...*......"
                           "..*.*....."
                           "...*......"
                           "...*......"
                           "..........";
    auto actual = gameOfLife(textInput, 3);
    EXPECT_EQ(actual, expected);
}

TEST(GameOfLife, Generation4) {
    std::string expected = ".........."
                           "...**....."
                           "..*..*...."
                           "..*......."
                           "..**......"
                           "..***....."
                           "..*.*....."
                           "..***....."
                           ".........."
                           "..........";
    auto actual = gameOfLife(textInput, 4);
    EXPECT_EQ(actual, expected);
}

TEST(GameOfLife, Generation5) {
    std::string expected = ".........."
                           "...**....."
                           "..*.*....."
                           ".**......."
                           ".*..*....."
                           ".*..*....."
                           ".*...*...."
                           "..*.*....."
                           "...*......"
                           "..........";
    auto actual = gameOfLife(textInput, 5);
    EXPECT_EQ(actual, expected);
}

TEST(GameOfLife, Generation6) {
    std::string expected = ".........."
                           "...**....."
                           ".**.*....."
                           ".**......."
                           "**.*......"
                           "***.**...."
                           ".*****...."
                           "..***....."
                           "...*......"
                           "..........";
    auto actual = gameOfLife(textInput, 6);
    EXPECT_EQ(actual, expected);
}

TEST(GameOfLife, Generation7) {
    std::string expected = ".........."
                           "..***....."
                           ".*..*....."
                           ".........."
                           "...**....."
                           ".....*...."
                           "*........."
                           ".*...*...."
                           "..***....."
                           "..........";
    auto actual = gameOfLife(textInput, 7);
    EXPECT_EQ(actual, expected);
}

TEST(GameOfLife, Generation8) {
    std::string expected = "...*......"
                           "..***....."
                           "..*.*....."
                           "...**....."
                           "....*....."
                           "....*....."
                           ".........."
                           ".****....."
                           "..***....."
                           "...*......";
    auto actual = gameOfLife(textInput, 8);
    EXPECT_EQ(actual, expected);
}

TEST(GameOfLife, Generation9) {
    std::string expected = "..***....."
                           "..*.*....."
                           "..*..*...."
                           "....**...."
                           "....**...."
                           ".........."
                           "..*.*....."
                           ".*..*....."
                           ".*........"
                           "..***.....";
    auto actual = gameOfLife(textInput, 9);
    EXPECT_EQ(actual, expected);
}

TEST(GameOfLife, Generation10) {
    std::string expected = "..*.*....."
                           ".**.**...."
                           ".....*...."
                           "...*..*..."
                           "....**...."
                           "...***...."
                           "...*......"
                           ".***......"
                           ".*..*....."
                           "..**......";
    auto actual = gameOfLife(textInput, 10);
    EXPECT_EQ(actual, expected);
}