<div align="center">
  <p>Balint Taschner | Kacper Urbaniec | PPR | 06.04.2023</p>
  <h1><ins>Game of Life</ins></h1>
</div>

## TODO Report

Bad benchmarks

* too small size
* intel turbo boost
  * fewer cores => higher boost clock
  * speedup decreases
  * disabling increases from speedup ~3.7 to ~5

Improvements

* std vec bool => std vec unsigned char => bool[]

  * 

  * ```
    // Alias `Cell` must not be of type `bool`!
    // `std::vector<bool>` stores multiple values in 1 byte.
    // Think about it like a compressed storage system, where every boolean value needs 1 bit.
    // So, instead of having one element per memory block (one element per array cell),
    // the memory layout may look like this:
    //    std::vector<bool> v(20);
    //    [ v[0], v[1], v[2],..., v[7] ][ v[8], v[9],..., v[15] ][ v[16], v[17], v[18], v[19] ]
    // => Most efficient type that stores one element at each index seems `unsigned char` (1 byte)
    // => How did was debugged? std::vector<bool> has no `data()` member to return pointer to first element
    // https://stackoverflow.com/a/46115714
    // https://stackoverflow.com/a/32821197
    // But generally it is save to write to vectors from multiple threads
    // unless no resizing is made
    // https://stackoverflow.com/a/9954045
    // https://stackoverflow.com/a/2951386
    ```

  * vec should have same performance as array when not resizing but rewriting gave a small boost

* writing all values to temporary grid is faster than reseting temporary grid to dead and setting only alive values

* false sharing padding

  * Did not observe any performance impact 

* ALIVE, DEAD constants

  * using const bool is way faster than define ALIVE true
  * have not found any explanation, global bool is shared via all threads wheres define is compiler punched
  * maybe compiler optimizations?

* omp no difference

  ```
  #pragma omp parallel default(none) firstprivate(generations, size) shared(oldGrid, newGrid)
              {
                  for (auto g = 0; g < generations; ++g) {
                      nextGeneration(size, oldGrid, newGrid);
                      // swapAndResetNewGrid(oldGrid, newGrid);
  
                      #pragma omp single
                      {
                          std::swap(oldGrid, newGrid);
                      };
              }
  }
  
  #pragma omp for schedule(static)
          for (auto i = 0; i < size; ++i) {
              auto startIndex = size + 3 + 64 + (i * 2) + (i * 64) + (i * size);
              for (auto j = 0; j < size; ++j) {
                  auto pos = startIndex + j;
                  auto aliveNeighbours = neighbourCount(pos, size, oldGrid);
                  toBeOrNotToBe(pos, aliveNeighbours, oldGrid, newGrid);
              }
          }
          
  ```

  ```
  
  
  
  void nextGeneration(int size, CellA* oldGrid, CellA* newGrid) {
          #pragma omp parallel for collapse(1) \
          schedule(static) \
          default(none) firstprivate(size, oldGrid, newGrid)
          for (auto i = 0; i < size; ++i) {
              auto startIndex = size + 3 + 64 + (i * 2) + (i * 64) + (i * size);
              for (auto j = 0; j < size; ++j) {
                  auto pos = startIndex + j;
                  auto aliveNeighbours = neighbourCount(pos, size, oldGrid);
                  toBeOrNotToBe(pos, aliveNeighbours, oldGrid, newGrid);
              }
          }
      }
  ```

  

New Benchmarks:



## Test

TODO describe how to run tests

## 🛠️ Build

```bash
mkdir cmake-build-release
cmake -DCMAKE_BUILD_TYPE=Release -B cmake-build-release/ .
cmake --build cmake-build-release/ --target game_of_life -- -j 9
```

## 🚀 Run

### Server Mode

Launch server

```bash
cmake-build-release/
.\game_of_lifet.exe [server] [optional: port]
# .\game_of_life.exe "server" "1234"
```

Launch client

```bash
cd ui/
./gradlew run
```

#### API

Interactive Mode:

1. Client initiates

   ```json
   {
   	input: {
           size: int,		// size of grid
           grid: [[bool]]	// full grid, dead=false/alive=true
       },
       // OPTIONAL
       threadConfig: {
           dynamic: bool,
           threadCount: int
       }
   }
   ```

2. Server returns diff update

   ```json
   {
       generation: int,
       diff: [int, [int]] // "Map", contains rows and indices which need to be flipped from last grid
   }
   ```

   > Example: Diff `[[2, [4, 5]], [5, [7]]]` means flip (alive/dead=>dead/alive) indices 4 & 5 at row 2 and flip index 7 at row 5. Rows & indices start at 0.

3. Client acknowledges

   ```json
   {
   	next: bool // Continue to next generation?
   }
   ```

Benchmark Mode:

1. Client initiates

   ```json
   {
   	input: {
           size: int,		// size of grid
           grid: [[bool]]	// full grid, dead=false/alive=true
       },
       {
       	iterations: int, // how many times should the algorithm be looped
       	generations: int, // how many generations should be calculated
   	},
       // OPTIONAL
       threadConfig: {
           dynamic: bool,
           threadCount: int
       }
   }
   ```

2. Server returns result

   ```json
   {
   	grid: [[bool]], // full grid, dead=false/alive=true
       averageTime: double,
       minTime: double,
       maxTime: double
   }
   ```

   

### Benchmark Mode

```bash
cmake-build-release/
.\game_of_lifet.exe [benchmark] [size] [generations] [iterations] [thread counts]
# .\game_of_life.exe "benchmark" "1024" "2000" "10" "1|2"
```

> Thread counts are separated with `|`.



## Sources

* Theory slide deck
* https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
* https://www.geeksforgeeks.org/program-for-conways-game-of-life/
* https://playgameoflife.com/
* https://github.com/nlohmann/json
* https://json.nlohmann.me/features/arbitrary_types/
* https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T
* https://stackoverflow.com/questions/46115669/why-does-stdvectorbool-have-no-data