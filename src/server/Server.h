//
// Created by kurbaniec on 27.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_SERVER_H
#define PPR_GAME_OF_LIFE_SERVER_H

#include "asio.hpp"
#include "json.hpp"
#include "../gameoflife/types/GameInput.h"
#include "../gameoflife/types/ThreadConfig.h"
#include "../gameoflife/types/BenchmarkInput.h"
#include <functional>
#include <optional>
#include <iostream>
#include <csignal>

using json = nlohmann::json;

namespace {
    std::optional<std::function<void(int)>> shutdownHandler;
    void signalHandler(int signal);
    std::string read(asio::ip::tcp::socket& socket);
    void send(asio::ip::tcp::socket& socket, const std::string& message);

    void interactive(
        const GameOfLife::GameInput& input,
        const std::optional<GameOfLife::ThreadConfig>& threadConfig,
        asio::ip::tcp::socket& socket
    );

    void benchmark(
        const GameOfLife::GameInput& input,
        const std::optional<GameOfLife::ThreadConfig>& threadConfig,
        const GameOfLife::BenchmarkInput& benchmarkInput,
        asio::ip::tcp::socket& socket
    );
}

namespace Server {
    void run(int port);
}


#endif //PPR_GAME_OF_LIFE_SERVER_H
