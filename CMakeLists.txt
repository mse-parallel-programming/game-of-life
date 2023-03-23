cmake_minimum_required(VERSION 3.24)
project(ppr_game_of_life)

set(CMAKE_CXX_STANDARD 20)
# Fixes "Missing openmp dll" as it will be included in the binary
# https://stackoverflow.com/a/52892848
# https://stackoverflow.com/a/18138926
set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

add_executable(game_of_life src/main.cpp src/global.cpp src/global.h src/sequential/GameOfLifeSeq.cpp src/sequential/GameOfLifeSeq.h src/GameOfLife.h src/util/Input.h src/parallel/GameOfLifePar.cpp src/parallel/GameOfLifePar.h)

# OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(game_of_life PUBLIC OpenMP::OpenMP_CXX)
endif()
