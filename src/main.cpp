#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <thread>
#include "global.h"
#include "gameoflife/GameOfLife.h"
#include "util/input.h"
#include "asio.hpp"
#include "json.hpp"
#include "messages/Start.h"
#include "messages/Update.h"
#include "server/Server.h"


/*std::string read(asio::ip::tcp::socket& socket) {
    asio::streambuf buf;
    asio::read_until(socket, buf, "\n");
    auto data = asio::buffer_cast<const char*>(buf.data());
    return data;
}

void send(asio::ip::tcp::socket& socket, const std::string& message) {
    const std::string terminatedMsg = message + "\n";
    asio::write(socket, asio::buffer(terminatedMsg));
}*/



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

    // const auto input = Util::textInput(textInput);
    // auto size = input.size;


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
    /*asio::io_service ioService;
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
    auto msgStruct = j.get<Message::Start>();

    std::cout << msgStruct.input.size << std::endl;
    // TODO: Start benchmark mode when benchmark input is not empty
    if (auto benchmarkInput = msgStruct.benchmarkInput) {
        std::cout << benchmarkInput->iterations << std::endl;
        std::cout << benchmarkInput->generations << std::endl;
    }
    // TODO: No thread config => use default, optimal values (dynamic=true, threadCount=enviroment.CoreCount)
    if (auto threadConfig = msgStruct.threadConfig) {
        std::cout << threadConfig->dynamic << std::endl;
        std::cout << threadConfig->threadCount << std::endl;
    }

    auto size = msgStruct.input.size;
    auto socketCallback = [&socket](
        int generation, int size,
        std::vector<Cell>& oldGrid,
        std::vector<Cell>& newGrid
    ) {
        auto updateMsg = Message::Update::from(generation, size, oldGrid, newGrid);
        nlohmann::json updateMsgJson = updateMsg;
        std::cout << updateMsgJson.dump() << std::endl;

        try {
            send(socket, updateMsgJson.dump());
            auto response = read(socket);
            if (response == "end!") return false;
            return true;
        } catch (...) {
            return false;
        }

    };

    GameOfLife::run(msgStruct.input, socketCallback);
    socket.close();*/

    /*std::cout << size << std::endl;

    auto iterations = 0;
    auto printCallback = [size, &iterations](
        std::vector<Cell>& oldGrid,
        std::vector<Cell>& newGrid
    ) {
        // std::stringstream out;
        // for (auto i = 0; i < size; ++i) {
        //     auto startIndex = size + 3 + (i * 2) + (i * size);
        //     for (auto j = 0; j < size; ++j) {
        //         auto pos = startIndex + j;
        //         auto cell = newGrid[pos];
        //         out << cell << " ";
        //     }
        //     out << " \n";
        // }
        // std::cout << out.str() << std::endl;

        ++iterations;
        if (iterations == 10) return false;
        return true;
    };
    GameOfLife::run(input, printCallback);*/


    Server::run(1234);
}
