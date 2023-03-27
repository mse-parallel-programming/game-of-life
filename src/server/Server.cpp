//
// Created by kurbaniec on 27.03.2023.
//


#include "Server.h"
#include "../messages/Start.h"
#include "../messages/Update.h"
#include "../gameoflife/GameOfLife.h"

namespace {
    void signalHandler(int signal) {
        std::cout << "signalHandler" << std::endl;
        if (shutdownHandler) (*shutdownHandler)(signal);
        std::cout << "signalHandler2" << std::endl;
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
}

namespace Server {
    void run(int port) {
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
            std::cout << "hey" << std::endl;
            listening = false;
            acceptor.close();
            socket.close();
            std::cout << "hey2" << std::endl;
        });

        while (listening) {
            std::cout << "a1" << std::endl;
            try {
                acceptor.accept(socket);
            } catch (...) {
                continue;
            }
            std::cout << "a2" << std::endl;

            auto startMsg = read(socket);
            auto start = json::parse(startMsg).get<Message::Start>();

            // TODO: Start benchmark mode when benchmark input is not empty
            if (auto benchmarkInput = start.benchmarkInput) {
                std::cout << benchmarkInput->iterations << std::endl;
                std::cout << benchmarkInput->generations << std::endl;
            }
            // TODO: No thread config => use default, optimal values (dynamic=true, threadCount=enviroment.CoreCount)
            if (auto threadConfig = start.threadConfig) {
                std::cout << threadConfig->dynamic << std::endl;
                std::cout << threadConfig->threadCount << std::endl;
            }

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
                    auto response = read(socket);
                    if (response == "end!\r\n") return false;
                    return true;
                } catch (...) {
                    return false;
                }
            };

            GameOfLife::run(start.input, socketCallback);

            socket.close();
        }
    }
}