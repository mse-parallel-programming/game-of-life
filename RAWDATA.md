<div align="center">
  <p>Balint Taschner | Kacper Urbaniec | PPR | 06.04.2023</p>
  <h1><ins>Game of Life</ins></h1>
</div>
## Raw Data

Benchmarks were performed on a mobile Intel i7-8750H CPU (6 Total Cores / 12 Total Threads). All runs besides the default one [set the thread count manually and disable OpenMP dynamic to limit used threads](https://stackoverflow.com/a/11096742). Turbo Boost was disabled for fairer comparisons.

### 512 Size / 1600 Generations / 10 Iterations

```
Game of Life
Parameters:
Size: 512
Generations: 1600
Iterations: 10
OpenMP Dynamic: 1
OpenMP Thread Count: 12
Benchmark result
Average time: 393.797 ms
Min time: 384.455 ms
Max time: 413.6 ms

Game of Life
Parameters:
Size: 512
Generations: 1600
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 1
Benchmark result
Average time: 702.679 ms
Min time: 690.971 ms
Max time: 716.008 ms

Game of Life
Parameters:
Size: 512
Generations: 1600
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 2
Benchmark result
Average time: 508.136 ms
Min time: 495.199 ms
Max time: 523.295 ms

Game of Life
Parameters:
Size: 512
Generations: 1600
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 4
Benchmark result
Average time: 377.241 ms
Min time: 362.841 ms
Max time: 394.905 ms

Game of Life
Parameters:
Size: 512
Generations: 1600
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 6
Benchmark result
Average time: 349.598 ms
Min time: 335.367 ms
Max time: 406.917 ms
```

### 1024 Size / 400 Generations / 10 Iterations

```
Game of Life
Parameters:
Size: 1024
Generations: 400
Iterations: 10
OpenMP Dynamic: 1
OpenMP Thread Count: 12
Benchmark result
Average time: 218.061 ms
Min time: 200.259 ms
Max time: 235.591 ms

Game of Life
Parameters:
Size: 1024
Generations: 400
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 1
Benchmark result
Average time: 692.601 ms
Min time: 677.946 ms
Max time: 713.322 ms

Game of Life
Parameters:
Size: 1024
Generations: 400
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 2
Benchmark result
Average time: 388.978 ms
Min time: 379.235 ms
Max time: 399.178 ms

Game of Life
Parameters:
Size: 1024
Generations: 400
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 4
Benchmark result
Average time: 236.653 ms
Min time: 221.195 ms
Max time: 280.285 ms

Game of Life
Parameters:
Size: 1024
Generations: 400
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 6
Benchmark result
Average time: 227.47 ms
Min time: 198.827 ms
Max time: 261.452 ms
```

### 2048 Size / 100 Generations / 10 Iterations

```
Game of Life
Parameters:
Size: 2048
Generations: 100
Iterations: 10
OpenMP Dynamic: 1
OpenMP Thread Count: 12
Benchmark result
Average time: 171.231 ms
Min time: 159.371 ms
Max time: 179.993 ms

Game of Life
Parameters:
Size: 2048
Generations: 100
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 1
Benchmark result
Average time: 701.773 ms
Min time: 690.556 ms
Max time: 713.627 ms

Game of Life
Parameters:
Size: 2048
Generations: 100
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 2
Benchmark result
Average time: 371.457 ms
Min time: 358.843 ms
Max time: 405.194 ms

Game of Life
Parameters:
Size: 2048
Generations: 100
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 4
Benchmark result
Average time: 205.324 ms
Min time: 190.38 ms
Max time: 249.349 ms

Game of Life
Parameters:
Size: 2048
Generations: 100
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 6
Benchmark result
Average time: 177.515 ms
Min time: 161.106 ms
Max time: 192.215 ms
```

### 5000 Size / 10 Generations / 10 Iterations

```
Game of Life
Parameters:
Size: 5000
Generations: 10
Iterations: 10
OpenMP Dynamic: 1
OpenMP Thread Count: 12
Benchmark result
Average time: 92.2032 ms
Min time: 80.3587 ms
Max time: 106.926 ms

Game of Life
Parameters:
Size: 5000
Generations: 10
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 1
Benchmark result
Average time: 415.156 ms
Min time: 406.085 ms
Max time: 427.177 ms

Game of Life
Parameters:
Size: 5000
Generations: 10
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 2
Benchmark result
Average time: 214.377 ms
Min time: 211.193 ms
Max time: 219.212 ms

Game of Life
Parameters:
Size: 5000
Generations: 10
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 4
Benchmark result
Average time: 120.591 ms
Min time: 111.922 ms
Max time: 129.156 ms

Game of Life
Parameters:
Size: 5000
Generations: 10
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 6
Benchmark result
Average time: 109.285 ms
Min time: 89.2889 ms
Max time: 129.085 ms
```

### 10000 Size / 10 Generations / 10 Iterations

```
Game of Life
Parameters:
Size: 10000
Generations: 10
Iterations: 10
OpenMP Dynamic: 1
OpenMP Thread Count: 12
Benchmark result
Average time: 336.798 ms
Min time: 309.566 ms
Max time: 368.869 ms

Game of Life
Parameters:
Size: 10000
Generations: 10
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 1
Benchmark result
Average time: 1652.93 ms
Min time: 1619.76 ms
Max time: 1683.77 ms

Game of Life
Parameters:
Size: 10000
Generations: 10
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 2
Benchmark result
Average time: 860.929 ms
Min time: 846.211 ms
Max time: 884.981 ms

Game of Life
Parameters:
Size: 10000
Generations: 10
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 4
Benchmark result
Average time: 464.382 ms
Min time: 427.849 ms
Max time: 501.813 ms

Game of Life
Parameters:
Size: 10000
Generations: 10
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 6
Benchmark result
Average time: 388.721 ms
Min time: 362.658 ms
Max time: 415.185 ms
```

### 20000 Size / 10 Generations / 10 Iterations

```
Game of Life
Parameters:
Size: 20000
Generations: 10
Iterations: 10
OpenMP Dynamic: 1
OpenMP Thread Count: 12
Benchmark result
Average time: 1280.78 ms
Min time: 1214.71 ms
Max time: 1367.72 ms

Game of Life
Parameters:
Size: 20000
Generations: 10
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 1
Benchmark result
Average time: 6610.75 ms
Min time: 6561.21 ms
Max time: 6673.47 ms

Game of Life
Parameters:
Size: 20000
Generations: 10
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 2
Benchmark result
Average time: 3381.6 ms
Min time: 3358.91 ms
Max time: 3429.94 ms

Game of Life
Parameters:
Size: 20000
Generations: 10
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 4
Benchmark result
Average time: 1838.91 ms
Min time: 1784.07 ms
Max time: 1878.16 ms

Game of Life
Parameters:
Size: 20000
Generations: 10
Iterations: 10
OpenMP Dynamic: 0
OpenMP Thread Count: 6
Benchmark result
Average time: 1498.45 ms
Min time: 1384.48 ms
Max time: 1556.93 ms
```

