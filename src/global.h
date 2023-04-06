//
// Created by kurbaniec on 22.03.2023.
//

#ifndef PPR_GAME_OF_LIFE_GLOBAL_H
#define PPR_GAME_OF_LIFE_GLOBAL_H

// Define constants
// https://stackoverflow.com/questions/2268749/defining-global-constant-in-c
extern const bool ALIVE;
extern const bool DEAD;

typedef bool Cell;

// Minimum offset between two objects to avoid false sharing
// https://en.cppreference.com/w/cpp/thread/hardware_destructive_interference_size
#include <new>
#ifdef __cpp_lib_hardware_interference_size
using std::hardware_constructive_interference_size;
using std::hardware_destructive_interference_size;
#else
// 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │ ...
constexpr std::size_t hardware_constructive_interference_size = 64;
constexpr std::size_t hardware_destructive_interference_size = 64;
#endif

const int falseSharingPadding = static_cast<int>(hardware_destructive_interference_size);

#endif //PPR_GAME_OF_LIFE_GLOBAL_H
