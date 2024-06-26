#ifndef KOSA_HPP
#define KOSA_HPP

#include <vector>
#include <stack>

class Graph {
public:
    int n; // Number of vertices
    int m; // Number of arcs
    std::vector<std::vector<int>> adj; // Adjacency list

    Graph();
    
    void NewEdge(int i, int j);
    void RemoveEdge(int i, int j);
    std::vector<std::vector<int>> Kosaraju();
    void NewGraph(int n, int m);
    void printOutput(const std::vector<std::vector<int>>& components);

private:
    void dfs1(int v, std::vector<bool>& visited, std::stack<int>& Stack);
    void dfs2(int v, std::vector<bool>& visited, const std::vector<std::vector<int>>& transpose, std::vector<int>& component);
    std::vector<std::vector<int>> getTranspose();
};

#endif // KOSA_HPP
