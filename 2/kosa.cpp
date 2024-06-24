#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include "kosa.hpp"

using namespace std;

Graph::Graph() : n(0), m(0) {}

void Graph::NewGraph(int n, int m, const std::vector<std::vector<int>>& edges) {
    this->n = n;
    this->m = m;
    this->adj.assign(n, std::vector<int>());

    for (const auto& edge : edges) {
        NewEdge(edge[0], edge[1]);
    }
}

void Graph::NewEdge(int i, int j) {
    if (i > n || j > n)
    {
        std::cout << "ERROR: bad vertice! , try again" << std::endl;
        std::cin >> i;
        std::cin >> j;
        NewEdge(i,j);
    }
    else {
        adj[i - 1].push_back(j - 1); // Convert to 0-based indexing
    }
}

void Graph::RemoveEdge(int i, int j) {
    auto& edges = adj[i - 1];
    edges.erase(std::remove(edges.begin(), edges.end(), j - 1), edges.end());
}

std::vector<std::vector<int>> Graph::Kosaraju() {
    std::cout << "Runs Kosaraju algorithm on the graph..." << std::endl;
    std::stack<int> Stack;
    std::vector<bool> visited(n, false);
    std::vector<std::vector<int>> components;

    // Step 1: Order vertices by finish time
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs1(i, visited, Stack);
        }
    }

    // Step 2: Transpose the graph
    std::vector<std::vector<int>> transpose = getTranspose();

    // Step 3: Find strongly connected components
    std::fill(visited.begin(), visited.end(), false);
    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();
        if (!visited[v]) {
            std::vector<int> component;
            dfs2(v, visited, transpose, component);
            for (int& vertex : component) {
                vertex += 1; // Convert back to 1-based indexing
            }
            components.push_back(component);
        }
    }
    return components;
}

void Graph::dfs1(int v, std::vector<bool>& visited, std::stack<int>& Stack) {
    visited[v] = true;
    for (int u : adj[v]) {
        if (!visited[u]) {
            dfs1(u, visited, Stack);
        }
    }
    Stack.push(v);
}

void Graph::dfs2(int v, std::vector<bool>& visited, const std::vector<std::vector<int>>& transpose, std::vector<int>& component) {
    visited[v] = true;
    component.push_back(v);
    for (int u : transpose[v]) {
        if (!visited[u]) {
            dfs2(u, visited, transpose, component);
        }
    }
}

std::vector<std::vector<int>> Graph::getTranspose() {
    std::vector<std::vector<int>> transpose(n);
    for (int v = 0; v < n; v++) {
        for (int u : adj[v]) {
            transpose[u].push_back(v);
        }
    }
    return transpose;
}