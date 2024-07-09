# Graph Implementation and Kosaraju's Algorithm

This program implements a directed graph and performs Kosaraju's algorithm to find strongly connected components.

## Notice!

   ### - this reademe is valid for step 4 to 10. earlier steps have their own readmes.
   
   ### - Netzer said we can have minor changes in the Protocol.We changed only the new graph command - in our project is one line command.

## Features

1. **Newgraph n,m**: Creates a new graph with `n` vertices and `m` edges.
2. **Kosaraju**: Runs Kosaraju's algorithm to find strongly connected components.
3. **Newedge i,j**: Adds a new edge from vertex `i` to vertex `j`.
4. **Removeedge i,j**: Removes the edge from vertex `i` to vertex `j`.
5. **exit**: Exits the program.

## How to Use

1. **Compile the Program && Run the Program**:
   ```sh
   make
   cd {STEP_NUMBER}
   ./main
   ```

2. **Initiate Connection With client(s)**
Open a new terminal
run nc as written below:
   ```sh
   nc localhost 9034
   ```
now you can use the graph commands!

## Commands

### Newgraph n,m

Creates a new graph with `n` vertices and `m` edges. The program will then prompt you to enter the `m` edges, one per line.

**Example**:
```
Newgraph 5,5 1,2 2,3 3,1 3,4 4,5
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

## tests

Run a test file to check the program.
Do it from different terminal of the running server.

```
make tests
```

---