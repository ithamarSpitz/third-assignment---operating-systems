#ifndef GRAPH_DEQUE_HPP
#define GRAPH_DEQUE_HPP

#include <vector>
#include <deque>
#include <stack>

class Vertex_Q {
public:
    int id;
    std::deque<int> children;

    Vertex_Q() : id(-1) {}
    Vertex_Q(int _id) : id(_id) {}
};

class Graph_deque {
public:
    Graph_deque();

    void NewEdge(int i, int j);
    void RemoveEdge(int i, int j);
    std::vector<std::vector<int>> Kosaraju();
    void NewGraph(int vertices, int edges, const std::vector<std::vector<int>>& edge);
    void printOutput(const std::vector<std::vector<int>>& components);

private:
    int n; // Number of vertices
    int m; // Number of arcs
    std::vector<Vertex_Q> vertices;
    std::vector<Vertex_Q> transpose;
    void dfs1(int v, std::vector<bool>& visited, std::stack<int>& Stack);
    void dfs2(int v, std::vector<bool>& visited, std::vector<int>& component);
    std::vector<Vertex_Q> getTranspose();
};

#endif // GRAPH_DEQUE_HPP
