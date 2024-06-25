#include "list_Kosa.hpp"
#include <algorithm>
#include <stack>
#include <iostream>
using namespace std;

Graph_list::Graph_list() : n(0), m(0) {}

void Graph_list::NewGraph(int vertices, int edges, const std::vector<std::vector<int>>& edge) {
    this->n = vertices;
    this->m = edges;
    this->vertices.clear();
    this->vertices.resize(n);
    
    for (int i = 0; i < n; i++) {
        this->vertices[i] = Vertex(i);
    }
    
    for (const auto& e : edge) {
        if (e.size() == 2) {
            int from = e[0] - 1;
            int to = e[1] - 1;
            if (from >= 0 && from < n && to >= 0 && to < n) {
                this->vertices[from].children.push_back(to);
            }
        }
    }

    std::cout << "Graph created with " << n << " vertices and " << m << " edges." << std::endl;
}

void Graph_list::NewEdge(int i, int j) {
    if (i > 0 && i <= n && j > 0 && j <= n) {
        vertices[i-1].children.push_back(j-1);
        m++;
    }
}

void Graph_list::RemoveEdge(int i, int j) {
    if (i > 0 && i <= n && j > 0 && j <= n) {
        vertices[i-1].children.remove(j-1);
        m--;
    }
}

std::vector<std::vector<int>> Graph_list::Kosaraju() {
    std::cout << "Runs Kosaraju algorithm on the graph..." << std::endl;
    std::stack<int> Stack;
    std::vector<bool> visited(n, false);

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfs1(i, visited, Stack);
        }
    }

    transpose = getTranspose();
    std::fill(visited.begin(), visited.end(), false);
    std::vector<std::vector<int>> result;

    while (!Stack.empty()) {
        int v = Stack.top();
        Stack.pop();
        if (!visited[v]) {
            std::vector<int> component;
            dfs2(v, visited, component);
            result.push_back(component);
        }
    }
    std::cout << "Kosaraju algorithm completed. Found " << result.size() << " SCCs." << std::endl;
    return result;
}

void Graph_list::dfs1(int v, std::vector<bool>& visited, std::stack<int>& Stack) {
    visited[v] = true;
    for (int child : vertices[v].children) {
        if (!visited[child]) {
            dfs1(child, visited, Stack);
        }
    }
    Stack.push(v);
}

void Graph_list::dfs2(int v, std::vector<bool>& visited, std::vector<int>& component) {
    visited[v] = true;
    component.push_back(v);
    for (int child : transpose[v].children) {
        if (!visited[child]) {
            dfs2(child, visited, component);
        }
    }
}

std::vector<Vertex> Graph_list::getTranspose() {
    std::vector<Vertex> trans(n);
    for (int i = 0; i < n; i++) {
        for (int child : vertices[i].children) {
            trans[child].children.push_back(i);
        }
    }
    return trans;
}

void Graph_list::printOutput(const std::vector<std::vector<int>>& components) {
    std::cout << "Output:" << std::endl;
    for (const auto& scc : components) {
        for (int vertex : scc) {
            std::cout << vertex + 1 << " ";
        }
        std::cout << std::endl;
    }
}
