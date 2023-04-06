<div align="center">
  <p>Balint Taschner | Kacper Urbaniec | PPR | 06.04.2023</p>
  <h1><ins>Game of Life</ins></h1>
</div>
## Problem domain

From [Wikipedia](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life):

*The Game of Life, also known simply as Life, is a cellular automaton devised by the British mathematician John Horton Conway in 1970. It is a zero-player game, meaning that its evolution is determined by its initial state, requiring no further input. One interacts with the Game of Life by creating an initial configuration and observing how it evolves. It is Turing complete and can simulate a universal constructor or any other Turing machine.* 

*The universe of the Game of Life is an infinite, two-dimensional orthogonal grid of square cells, each of which is in one of two possible states, live or dead (or populated and unpopulated, respectively). Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent. At each step in time, the following transitions occur:*

1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

*These rules, which compare the behaviour of the automaton to real life, can be condensed into the following:*

1. Any live cell with two or three live neighbours survives.
2. Any dead cell with three live neighbours becomes a live cell.
3. All other live cells die in the next generation. Similarly, all other dead cells stay dead.

## Approach

The parallelization approach is based on the explanations of section *Loop-based parallelism* from *Victor Eijkhout, Topics in Parallel and Distributed Computing, Chapter 10
Parallel Programming Illustrated Through Conway’s
Game of Life*. It is probably the most primitive approach and strongly resembles the sequential implementation. Since each iteration and evaluation within a generation can be updated independently, they can be executed in any order, or in fact simultaneously.  

The author proposes following pseudocode for the implementation:

```python
def life_generation( board,tmp ):
    # OMP parallel for
    for i in [0:N-1]:
    	for j in [0:N-1]:
    		tmp[i,j] = board[i,j]
	# OMP parallel for
	for i in [0:N-1]:
		for j in [0:N-1]:
			board[i,j] = life_evaluation( tmp[i-1:i+1,j-1:j+1] 
```

Our final solution bears strong resemblances, performing the parallelization also on rows via OpenMP, however instead of copying the board grid & temporary one, the pointer are just swapped. 

```c++
void nextGeneration(int size, CellA* oldGrid, CellA* newGrid) {
    #pragma omp parallel for collapse(1) \
    schedule(static) \
    default(none) firstprivate(size, oldGrid, newGrid)
    for (auto i = 0; i < size; ++i) {
        auto startIndex = GameOfLife::rowStartIndexAt(i, size);
        for (auto j = 0; j < size; ++j) {
            auto pos = startIndex + j;
            auto aliveNeighbours = neighbourCount(pos, size, oldGrid);
            toBeOrNotToBe(pos, aliveNeighbours, oldGrid, newGrid);
        }
    }
}
...
for (auto g = 0; g < generations; ++g) {
    nextGeneration(size, oldGrid, newGrid);
    std::swap(oldGrid, newGrid);
}
...
```

The downsides of the implementation & the general problem with the game of life is, that this parallelism is only possible within a generation as the neighbour counting can reference rows that are worked on by other threads. This is why a temporary grid for reading and a new grid for writing is used; the temporary grid is the snapshot of the last generation and basis for evaluation. This means after each generations the worker threads must be synchronized which can result in an overhead. The workers should finish each generation in a comparable time in theory, however nothing prevents the operation system to schedule something outside the application context, slowing one or more worker threads, delaying the synchronization and start of a new generation.

### Grid & Boundary Checks

Conway designed the game for an infinite grid. However, memory is finite so a strategy must be made. Our implementation uses the most basic solution, the provided grid cannot be resized and every cell outside the grid is assumed dead. 

To avoid border checking and improve performance the grid is padded around with dead cells. So an input grid of 3x3 becomes internally 5x5, but iterations are only performed on the original 3x3 indices. To be precise, our implementation uses an one-dimensional array that is logically handled like a two dimensional array which streamlines memory allocation/deallocation. Also, later implementations also introduced an padding for each logical row to be absolutely sure that writing operations on neighbouring rows evaluated by different worker threads do not invalidate cache lines. So the final representation of the grid can be visualized as the following:

# TODO illustration





To be absolutely sure that no false sharing is happening, 



### ``



## Sources

* https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
* Victor Eijkhout, Topics in Parallel and Distributed Computing, Chapter 10
  Parallel Programming Illustrated Through Conway’s
  Game of Life
