#ifndef GRAPH_LIST_HPP
#define GRAPH_LIST_HPP

#include <vector>
#include <list>
#include <stack>

class Graph_list {
public:
    int n; // Number of vertices
    int m; // Number of arcs
    std::vector<std::list<int>> adj; // Adjacency list using list

    Graph_list();

    void NewEdge(int i, int j);
    void RemoveEdge(int i, int j);
    std::vector<std::list<int>> Kosaraju();
    void NewGraph(int n, int m, const std::vector<std::list<int>>& edges);

private:
    void dfs1(int v, std::vector<bool>& visited, std::stack<int>& Stack);
    void dfs2(int v, std::vector<bool>& visited, const std::vector<std::list<int>>& transpose, std::list<int>& component);
    std::vector<std::list<int>> getTranspose();
};

#endif // GRAPH_LIST_HPP
