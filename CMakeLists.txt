cmake_minimum_required(VERSION 3.24)
project(ppr_game_of_life)

set(CMAKE_CXX_STANDARD 20)
# Fixes "Missing openmp dll" as it will be included in the binary
# https://stackoverflow.com/a/52892848
# https://stackoverflow.com/a/18138926
set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
# Asio
add_definitions(-DASIO_STANDALONE)
add_definitions(-DASIO_HAS_STD_ADDRESSOF)
add_definitions(-DASIO_HAS_STD_ARRAY)
add_definitions(-DASIO_HAS_CSTDINT)
add_definitions(-DASIO_HAS_STD_SHARED_PTR)
add_definitions(-DASIO_HAS_STD_TYPE_TRAITS)
add_definitions(-DASIO_HAS_VARIADIC_TEMPLATES)
add_definitions(-DASIO_HAS_STD_FUNCTION)
add_definitions(-DASIO_HAS_STD_CHRONO)
add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-D_WIN32_WINNT=0x0501)
add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)

add_library(game_of_life_lib src/global.cpp src/global.h src/gameoflife/GameOfLife.h src/util/Input.h src/gameoflife/types/BenchmarkResult.h src/gameoflife/types/BenchmarkInput.h src/gameoflife/types/GameInput.h src/gameoflife/GameOfLife.cpp src/messages/Start.h src/messages/Update.h src/gameoflife/types/ThreadConfig.h src/server/Server.cpp src/server/Server.h src/messages/Update.cpp src/messages/Start.cpp src/benchmark/Benchmark.cpp src/benchmark/Benchmark.h src/util/Parse.h src/messages/UpdateAck.cpp src/messages/UpdateAck.h src/messages/Benchmark.cpp src/messages/Benchmark.h)


# OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(game_of_life_lib PUBLIC OpenMP::OpenMP_CXX)
endif()
# Asio
include_directories(${PROJECT_SOURCE_DIR}/dependencies/asio-1.26.0/include)
# JSON for Modern C++
include_directories(${PROJECT_SOURCE_DIR}/dependencies/json-3.11.2)

# Test
add_subdirectory(test)

add_executable(game_of_life src/main.cpp)
target_link_libraries(game_of_life game_of_life_lib)