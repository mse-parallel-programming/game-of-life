# game-of-life

## TODO Report

Bad benchmarks

* too small size
* intel turbo boost
  * fewer cores => higher boost clock
  * speedup decreases
  * disabling increases from speedup ~3.7 to ~5

Improvements

* std vec bool => std vec unsigned char => bool[]
  * vec should have same performance as array when not resizing but rewriting gave a small boost
* false sharing padding
  * Did not observe any performance impact 
* ALIVE, DEAD constants
  * using const bool is way faster than define ALIVE true
  * have not found any explanation, global bool is shared via all threads wheres define is compiler punched
  * maybe compiler optimizations?

New Benchmarks:



## Test

TODO describe

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