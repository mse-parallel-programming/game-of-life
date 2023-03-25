#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <thread>
#include "global.h"
#include "gameoflife/GameOfLife.h"
#include "sequential/GameOfLifeSeq.h"
#include "util/input.h"
#include "parallel/GameOfLifePar.h"
#include "asio.hpp"
#include "json.hpp"
#include "messages/StartMessage.h"
#include "messages/UpdateMessage.h"


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
    // std::string textInput = ".........."
    //                         "...**....."
    //                         "....*....."
    //                         ".........."
    //                         ".........."
    //                         "...**....."
    //                         "..**......"
    //                         ".....*...."
    //                         "....*....."
    //                         "..........";
    // std::string textInput = "..."
    //                         ".*."
    //                         "...";

    // auto input = Util::textInput(textInput);

    // auto input = Util::randomInput(512, 0.25);
    // auto size = input.size;
    // auto grid = input.grid;

    // GameOfLifeSeq seqImpl;
    // GameOfLifePar parImpl;
    // GameOfLife& game = seqImpl;

    // auto printCallback = [size](const std::vector<Cell>& iterationGrid) {
    //     for (auto i = 0; i < size; ++i) {
    //         auto startIndex = size + 3 + (i * 2) + (i * size);
    //         for (auto j = 0; j < size; ++j) {
    //             auto pos = startIndex + j;
    //             auto cell = iterationGrid[pos];
    //             std::cout << cell << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    //     std::cout << std::endl;
    // };

    // seqImpl.run(15, size, grid, printCallback);
    // seqImpl.benchmark(10, 1000, size, grid);

    // omp_set_dynamic(0);
    // omp_set_num_threads(1);

    // std::cout << "baum " << omp_get_thread_limit() << std::endl;
    // std::cout << "baum " << omp_get_max_threads() << std::endl;
    // std::cout << "hey" << std::thread::hardware_concurrency();

    // parImpl.run(15, size, grid, printCallback);
    // parImpl.benchmark(10, 1000, size, grid);

    // auto gameInput = GameOfLife::GameInput {
    //     size, grid
    // };
    // auto benchmarkInput = GameOfLife::BenchmarkInput {
    //     10, 1000, true, 1
    // };
    // auto result = GameOfLife::benchmark(gameInput, benchmarkInput);
    // result.print();

    // https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T
    // https://sourceforge.net/projects/asio/files/asio/1.26.0%20%28Stable%29/asio-1.26.0.zip/download
    // https://stackoverflow.com/a/75828304
    asio::io_service ioService;
    asio::ip::tcp::acceptor acceptor(
        ioService,
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 1234)
    );
    asio::ip::tcp::socket socket(ioService);

    acceptor.accept(socket);

    std::cout << "Sending msg to client" << std::endl;
    send(socket, "Hey!");

    std::cout << "Waiting for reply" << std::endl;
    auto msg = read(socket);
    std::cout << msg << std::endl;


    auto test = std::map<int, std::vector<int>> { {0, {0, 1}}};
    nlohmann::json test2(test);
    std::cout << nlohmann::to_string(test2) << std::endl;


    nlohmann::json j = nlohmann::json::parse(msg);
    auto msgStruct = j.get<GameOfLife::StartMessage>();

    std::cout << msgStruct.input.size << std::endl;
    if (auto benchmarkInput = msgStruct.benchmarkInput) {
        std::cout << benchmarkInput->dynamic.has_value() << std::endl;
        std::cout << *benchmarkInput->dynamic << std::endl;
        std::cout << benchmarkInput->threadCount.has_value() << std::endl;
        std::cout << *benchmarkInput->threadCount << std::endl;
    }

    auto size = msgStruct.input.size;
    auto socketCallback = [&socket, size](
        std::vector<Cell>& oldGrid,
        std::vector<Cell>& newGrid
    ) {
        // TODO: Send json diff
        std::stringstream out;
        for (auto i = 0; i < size; ++i) {
            auto startIndex = size + 3 + (i * 2) + (i * size);
            for (auto j = 0; j < size; ++j) {
                auto pos = startIndex + j;
                auto cell = newGrid[pos];
                out << cell << " ";
            }
            out << ";";
        }

        auto updateMsg = GameOfLife::UpdateMessage::from(0, size, oldGrid, newGrid);
        nlohmann::json updateMsgJson = updateMsg;
        std::cout << updateMsgJson.dump() << std::endl;

        try {
            // send(socket, out.str());
            send(socket, updateMsgJson.dump());
            auto response = read(socket);
            if (response == "end!") return false;
            return true;
        } catch (...) {
            return false;
        }

    };

    GameOfLife::run(msgStruct.input, socketCallback);

    socket.close();
}
