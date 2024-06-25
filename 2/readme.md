# Graph Implementations and Profiling

This repository contains three different implementations of a directed graph data structure and the Kosaraju algorithm for finding Strongly Connected Components (SCCs). The implementations are:

1. **Using `std::vector<std::vector<int>>`**: This implementation uses an adjacency list representation with `std::vector`.
2. **Using `std::list<int>`**: This implementation uses an adjacency list representation with `std::list`.
3. **Using `std::deque<int>`**: This implementation uses an adjacency list representation with `std::deque`.

## Files

- **kosa.hpp and kosa.cpp**: Contains the implementation using `std::vector<std::vector<int>>`.
- **list_Kosa.hpp and list_Kosa.cpp**: Contains the implementation using `std::list<int>`.
- **deque_Kosa.hpp and deque_Kosa.cpp**: Contains the implementation using `std::deque<int>`.
- **main.cpp**: Contains the code to test and profile each graph implementation.

## Graph Implementations

### 1. `std::vector<std::vector<int>>` Implementation

- **File**: `kosa.hpp`, `kosa.cpp`
- **Class**: `Graph`
- **Description**: Uses `std::vector` to maintain the adjacency list for the graph.

### 2. `std::list<int>` Implementation

- **File**: `list_Kosa.hpp`, `list_Kosa.cpp`
- **Class**: `Graph_list`
- **Description**: Uses `std::list` to maintain the adjacency list for the graph.

### 3. `std::deque<int>` Implementation

- **File**: `deque_Kosa.hpp`, `deque_Kosa.cpp`
- **Class**: `Graph_deque`
- **Description**: Uses `std::deque` to maintain the adjacency list for the graph.

## Profiling

The `main.cpp` file contains code to profile the time taken by each implementation to run the Kosaraju algorithm on a generated graph. The graph used for testing has 1000 vertices and 5000 edges, which are randomly generated.

### How to Run

1. **Compile the Code && Run the Executable**:
   ```sh
   make run
   ```

### Output

The program will output the strongly connected components and the time taken to compute them for each graph implementation.


### Example Input

```
int n = 5;
int m = 5;

vector<vector<int>> edges = {
    {1, 2},
    {2, 3},
    {3, 1},
    {3, 4},
    {4, 5},
};
```

### Example Output

```
Testing with std::vector<int>:
Runs Kosaraju algorithm on the graph...
Strongly Connected Components:
Output:
1 3 2 
4 
5 
Time taken: X microseconds

Testing with std::deque<int>:
Graph created with 5 vertices and 5 edges.
Runs Kosaraju algorithm on the graph...
Kosaraju algorithm completed. Found 3 SCCs.
Strongly Connected Components:
Output:
1 3 2 
4 
5 
Time taken: X microseconds

Testing with std::list<int>:
Graph created with 5 vertices and 5 edges.
Runs Kosaraju algorithm on the graph...
Kosaraju algorithm completed. Found 3 SCCs.
Strongly Connected Components:
Output:
1 3 2 
4 
5 
Time taken: X microseconds
```

## Conclusion

This repository demonstrates three different ways to implement a graph data structure and how to profile the performance of each implementation. By running the provided `main.cpp`, you can observe the differences in execution time for each approach.

---