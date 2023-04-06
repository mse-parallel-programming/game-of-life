#include <iostream>
#include <vector>
#include "server/Server.h"
#include "benchmark/Benchmark.h"
#include "util/Parse.h"


int main(int argc, char* argv[]) {

    std::vector<std::string> args(&argv[0], &argv[0 + argc]);
    args.erase(args.begin());
    auto argsCount = args.size();

    // Determine runtime mode
    if (args[0] == "server" || argsCount == 0) {
        int port = 1234;
        if (argsCount >= 2) port = std::stoi(args[1]);
        Server::run(port);
    } else if (args[0] == "benchmark" && argsCount == 5) {
        int size = std::stoi(args[1]);
        int generations = std::stoi(args[2]);
        int iterations = std::stoi(args[3]);
        auto threads = Util::strToInt(Util::split(args[4], '|'));
        Benchmark::localBenchmark(size, generations, iterations, threads);
    } else {
        const char* usage =
            "Usage [server] [optional: port] or "
            "[benchmark] [size] [generations] [iterations] [thread counts]";
        throw std::runtime_error(usage);
    }
}
