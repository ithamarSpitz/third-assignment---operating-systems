#include "list_Kosa.hpp"
#include <algorithm>
#include <stack>

Graph_list::Graph_list() : n(0), m(0) {}

void Graph_list::NewGraph(int n, int m, const std::vector<std::list<int>>& edges) {
    this->n = n;
    this->m = m;
    this->adj.assign(n, std::list<int>());

    for (int i = 0; i < n; ++i) {
        for (int j : edges[i]) {
            NewEdge(i + 1, j + 1); // Convert to 1-based indexing
        }
    }
}

void Graph_list::NewEdge(int i, int j) {
    adj[i - 1].push_back(j - 1); // Convert to 0-based indexing
}

void Graph_list::RemoveEdge(int i, int j) {
    auto& edges = adj[i - 1];
    edges.remove(j - 1); // Using list's remove function for O(n) removal
}

std::vector<std::list<int>> Graph_list::Kosaraju() {
    std::stack<int> Stack;
    std::vector<bool> visited(n, false);
    std::vector<std::list<int>> components;

    // Step 1: Order vertices by finish time
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs1(i, visited, Stack);
        }
    }

    // Step 2: Transpose the graph
    std::vector<std::list<int>> transpose = getTranspose();

    // Step 3: Find strongly connected components
    std::fill(visited.begin(), visited.end(), false);
    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();
        if (!visited[v]) {
            std::list<int> component;
            dfs2(v, visited, transpose, component);
            components.push_back(component);
        }
    }
    return components;
}

void Graph_list::dfs1(int v, std::vector<bool>& visited, std::stack<int>& Stack) {
    visited[v] = true;
    for (int u : adj[v]) {
        if (!visited[u]) {
            dfs1(u, visited, Stack);
        }
    }
    Stack.push(v);
}

void Graph_list::dfs2(int v, std::vector<bool>& visited, const std::vector<std::list<int>>& transpose, std::list<int>& component) {
    visited[v] = true;
    component.push_back(v);
    for (int u : transpose[v]) {
        if (!visited[u]) {
            dfs2(u, visited, transpose, component);
        }
    }
}

std::vector<std::list<int>> Graph_list::getTranspose() {
    std::vector<std::list<int>> transpose(n);
    for (int v = 0; v < n; v++) {
        for (int u : adj[v]) {
            transpose[u].push_back(v);
        }
    }
    return transpose;
}
