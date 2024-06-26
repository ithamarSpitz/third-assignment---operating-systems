#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include "kosa.hpp"
using namespace std;

Graph::Graph() : n(0), m(0) {}

void Graph::NewGraph(int n, int m) {
    this->n = n;
    this->m = m;
    this->adj.assign(n, std::vector<int>());
    int i = -1; 
    int j = -1;

    std::cout << "enter the edges:" << std::endl;
    for (int k = 0; k < m; k++) {
        std::cout << "edge" << k+1 << ": " << std::endl;
        std::cin >> i >> j;
        NewEdge(i, j);   
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

void Graph::printOutput(const std::vector<std::vector<int>>& components) {
    std::cout << "Output:" << std::endl;
    for (const auto& component : components) {
        for (int vertex : component) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    }
}

vector<string> Graph::parse(const string& command) {
    vector<string> parts;
    istringstream iss(command);
    string part;
    
    // Split command based on space and comma
    while (getline(iss, part, ' ')) {
        parts.push_back(part);
    }

    if (parts.size() > 1) {
        istringstream iss_args(parts[1]);
        vector<string> args;
        // Split command args based on ','
        while (getline(iss_args, part, ',')) {
            args.push_back(part);
        }
        parts.erase(parts.begin() + 1);
        parts.insert(parts.end(), args.begin(), args.end());
    }

    // Check if the command is valid
    if (!parts.empty()) {
        string cmd = parts[0];
        if (cmd == "Newgraph" && parts.size() == 3) {
            return parts;
        } else if (cmd == "Kosaraju" && parts.size() == 1) {
            return parts;
        } else if ((cmd == "Newedge" || cmd == "Removeedge") && parts.size() == 3) {
            return parts;
        } else if (cmd == "exit" && parts.size() == 1) {
            return parts;
        }
    }

    // If we reach here, the command is not valid
    cerr << "Invalid command: " << command << endl;
    return vector<string>();
}

bool Graph::eval(Graph& graph, const vector<string>& parts) {
    const string& cmd = parts[0];

    if (cmd == "exit") {
        return false;
    } 
    if (cmd == "Newgraph") {
        int n = stoi(parts[1]);
        int m = stoi(parts[2]);
        graph.NewGraph(n, m);
    } else if (cmd == "Kosaraju") {
        vector<vector<int>> components = graph.Kosaraju();
        graph.printOutput(components);
    } else if (cmd == "Newedge") {
        int i = stoi(parts[1]);
        int j = stoi(parts[2]);
        graph.NewEdge(i, j);
    } else if (cmd == "Removeedge") {
        int i = stoi(parts[1]);
        int j = stoi(parts[2]);
        graph.RemoveEdge(i, j);
    }
    
    return true;
}
