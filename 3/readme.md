# Graph Implementation and Kosaraju's Algorithm

This program implements a directed graph and performs Kosaraju's algorithm to find strongly connected components. The graph supports interactive commands via standard input (stdin).

## Features

1. **Newgraph n,m**: Creates a new graph with `n` vertices and `m` edges.
2. **Kosaraju**: Runs Kosaraju's algorithm to find strongly connected components.
3. **Newedge i,j**: Adds a new edge from vertex `i` to vertex `j`.
4. **Removeedge i,j**: Removes the edge from vertex `i` to vertex `j`.
5. **exit**: Exits the program.

## How to Use

1. **Compile the Program && Run the Program**:
   ```sh
   make run
   ```

2. **Enter Commands**: The program reads commands from stdin. Each command should be entered on a new line.

## Commands

### Newgraph n,m

Creates a new graph with `n` vertices and `m` edges. The program will then prompt you to enter the `m` edges, one per line.

**Example**:
```
Newgraph 5,5
```

**Expected Input After Command**:
```
1 2
2 3
3 1
3 4
4 5
```

### Kosaraju

Runs Kosaraju's algorithm on the current graph and outputs the strongly connected components.

**Example**:
```
Kosaraju
```

### Newedge i,j

Adds an edge from vertex `i` to vertex `j`.

**Example**:
```
Newedge 2,5
```

### Removeedge i,j

Removes the edge from vertex `i` to vertex `j`.

**Example**:
```
Removeedge 2,5
```

### exit

Exits the program.

**Example**:
```
exit
```

### Explanation of Changes

- **Command Parsing**: The `parse` function splits the input command by spaces and commas to handle commands like `Newedge i,j` correctly.
- **Command Evaluation**: The `eval` function executes the corresponding graph operations based on the parsed command. It returns `false` for the `exit` command to signal termination of the program.
- **Main Loop**: The `main` function reads commands in a loop and processes them using the `parse` and `eval` functions. The loop breaks and the program exits when `eval` returns `false`.

---