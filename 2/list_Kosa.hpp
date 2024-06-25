#ifndef GRAPH_LIST_HPP
#define GRAPH_LIST_HPP

#include <vector>
#include <list>
#include <stack>

class Vertex {
public:
    int id;
    std::list<int> children;

    Vertex() : id(-1) {}
    Vertex(int _id) : id(_id) {}
};

class Graph_list {
public:
    Graph_list();

    void NewEdge(int i, int j);
    void RemoveEdge(int i, int j);
    std::vector<std::vector<int>> Kosaraju();
    void NewGraph(int vertices, int edges, const std::vector<std::vector<int>>& edge);
    void printOutput(const std::vector<std::vector<int>>& components);

private:
    int n; // Number of vertices
    int m; // Number of arcs
    std::vector<Vertex> vertices;
    std::vector<Vertex> transpose;
    void dfs1(int v, std::vector<bool>& visited, std::stack<int>& Stack);
    void dfs2(int v, std::vector<bool>& visited, std::vector<int>& component);
    std::vector<Vertex> getTranspose();
};

#endif // GRAPH_LIST_HPP
