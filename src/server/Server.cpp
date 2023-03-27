//
// Created by kurbaniec on 27.03.2023.
//


#include "Server.h"
#include "../messages/Start.h"
#include "../messages/Update.h"
#include "../gameoflife/GameOfLife.h"
#include "../messages/UpdateAck.h"
#include "../messages/Benchmark.h"

namespace {
    void signalHandler(int signal) {
        if (shutdownHandler) (*shutdownHandler)(signal);
    }

    std::string read(asio::ip::tcp::socket& socket) {
        asio::streambuf buf;
        asio::read_until(socket, buf, "\n");
        auto data = asio::buffer_cast<const char*>(buf.data());
        return data;
    }

    void send(asio::ip::tcp::socket& socket, const std::string& message) {
        const std::string terminatedMsg = message + "\n";
        asio::write(socket, asio::buffer(terminatedMsg));
    }

    void interactive(
        const GameOfLife::GameInput& input,
        const std::optional<GameOfLife::ThreadConfig>& threadConfig,
        asio::ip::tcp::socket& socket
    ) {
        auto socketCallback = [&socket](
            int generation, int size,
            std::vector<Cell>& oldGrid,
            std::vector<Cell>& newGrid
        ) {
            auto updateMsg = Message::Update::from(generation, size, oldGrid, newGrid);
            json updateMsgJson = updateMsg;
            std::cout << updateMsgJson.dump() << std::endl;

            try {
                send(socket, updateMsgJson.dump());
                auto responseJson = read(socket);
                auto response = json::parse(responseJson).get<Message::UpdateAck>();
                return response.next;
            } catch (...) {
                return false;
            }
        };

        GameOfLife::run(input, threadConfig, socketCallback);
    }

    void benchmark(
        const GameOfLife::GameInput& input,
        const std::optional<GameOfLife::ThreadConfig>& threadConfig,
        const GameOfLife::BenchmarkInput& benchmarkInput,
        asio::ip::tcp::socket& socket
    ) {
        auto result = GameOfLife::benchmarkWithGrid(input, threadConfig, benchmarkInput);
        auto resultMsg = Message::Benchmark(result);
        json resultMsgJson = resultMsg;

        try {
            send(socket, resultMsgJson.dump());
        } catch (...) {
            std::cout << "Something went wrong sending benchmark results" << std::endl;
        }
    }
}

namespace Server {
    void run(int port) {
        std::cout << "Starting Game of Life server..." << std::endl;
        std::cout << "Running on port... " << port << std::endl;

        // Shutdown handler
        // https://stackoverflow.com/a/48164204
        std::signal(SIGINT, signalHandler);

        // Server
        // https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T
        // https://sourceforge.net/projects/asio/files/asio/1.26.0%20%28Stable%29/asio-1.26.0.zip/download
        // https://stackoverflow.com/a/75828304
        asio::io_service ioService;
        asio::ip::tcp::acceptor acceptor(
            ioService,
            asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)
        );
        asio::ip::tcp::socket socket(ioService);

        auto listening = true;
        shutdownHandler = std::make_optional([&listening, &socket, &acceptor](int signal) {
            listening = false;
            acceptor.close();
            socket.close();
            std::cout << "Initiating shutdown..." << std::endl;
        });

        while (listening) {
            std::cout << std::endl << "Waiting for new connection..." << std::endl;
            try {
                acceptor.accept(socket);
            } catch (...) {
                continue;
            }

            auto startMsg = read(socket);
            auto start = json::parse(startMsg).get<Message::Start>();
            auto input = start.input;
            auto threadConfig = start.threadConfig;

            // TODO: Start benchmark mode when benchmark input is not empty
            if (auto benchmarkInput = start.benchmarkInput) {
                // benchmark mode
                benchmark(input, threadConfig, *benchmarkInput, socket);
            } else {
                // Interactive mode
                interactive(input, threadConfig, socket);
            }

            // auto socketCallback = [&socket](
            //     int generation, int size,
            //     std::vector<Cell>& oldGrid,
            //     std::vector<Cell>& newGrid
            // ) {
            //     auto updateMsg = Message::Update::from(generation, size, oldGrid, newGrid);
            //     json updateMsgJson = updateMsg;
            //     std::cout << updateMsgJson.dump() << std::endl;
            //
            //     try {
            //         send(socket, updateMsgJson.dump());
            //         auto responseJson = read(socket);
            //         auto response = json::parse(responseJson).get<Message::UpdateAck>();
            //         return response.next;
            //     } catch (...) {
            //         return false;
            //     }
            // };
            //
            // GameOfLife::run(start.input, start.threadConfig, socketCallback);


            socket.close();
        }

        std::cout << "Bye!" << std::endl;
    }
}