# Strongly Connected Components Detection using Kosaraju's Algorithm

This project demonstrates the implementation of Kosaraju's algorithm to find all strongly connected components (SCCs) in a directed graph.

## Overview

The project consists of the following files:

- **kosa.hpp, kosa.cpp**: Header and implementation files for the `Graph` class, which manages graph operations including adding edges, removing edges, and finding SCCs using Kosaraju's algorithm.
**Using `std::vector<std::vector<int>>`**: This implementation uses an adjacency list representation with `std::vector`.
  
- **main.cpp**: Main program file where the user can input the number of vertices, number of edges, and the edges themselves. It initializes a `Graph` object, populates it with the input edges, runs Kosaraju's algorithm, and prints out the SCCs found.

- **makefile**: Makefile for compiling the project. It compiles `main.cpp` and `kosa.cpp` into an executable named `main`.

## Usage

To compile the project, run:

```bash
make
```

This will generate an executable named `main`.

To execute the program, run:

```bash
./main <vertices> <edges>
```

- `<vertices>`: Number of vertices in the graph.
- `<edges>`: Number of edges in the graph.

Follow the prompts to enter each edge. If an invalid vertex is entered, the program will prompt for re-entry.

## Example

Suppose you have a graph with 4 vertices and 3 edges:

```bash
./main 5 5
```

Input the edges when prompted:

```plaintext
edge 1:
1 2
edge 2:
2 3
edge 3:
3 1
edge 4:
3 4
edge 5:
4 5
```

```output
Runs Kosaraju algorithm on the graph...
Output:
1 3 2 
4 
5 
```
---