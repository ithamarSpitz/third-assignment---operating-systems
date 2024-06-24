#ifndef GRAPH_DEQUE_HPP
#define GRAPH_DEQUE_HPP

#include <vector>
#include <deque>
#include <stack>

class Graph_deque {
public:
    int n; // Number of vertices
    int m; // Number of arcs
    std::vector<std::deque<int>> adj; // Adjacency list using deque

    Graph_deque();

    void NewEdge(int i, int j);
    void RemoveEdge(int i, int j);
    std::vector<std::deque<int>> Kosaraju();
    void NewGraph(int n, int m, const std::vector<std::deque<int>>& edges);

private:
    void dfs1(int v, std::vector<bool>& visited, std::stack<int>& Stack);
    void dfs2(int v, std::vector<bool>& visited, const std::vector<std::deque<int>>& transpose, std::deque<int>& component);
    std::vector<std::deque<int>> getTranspose();
};

#endif // GRAPH_DEQUE_HPP
