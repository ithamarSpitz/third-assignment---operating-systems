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
}

void Graph::NewEdge(int i, int j) {
    if (i > n || j > n) return;
    else {
        adj[i - 1].push_back(j - 1); 
    }
}

void Graph::RemoveEdge(int i, int j) {
    auto& edges = adj[i - 1];
    edges.erase(std::remove(edges.begin(), edges.end(), j - 1), edges.end());
}

std::vector<std::vector<int>> Graph::Kosaraju() {
    // std::cout << "Runs Kosaraju algorithm on the graph..." << std::endl;
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
    std::cout << "Kosaraju Output:" << std::endl;
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
    // If command is "Newgraph" don't split edges (will do it automatically in eval)
    if (parts.size() > 1 && parts[0] != "Newgraph") {
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
        if (cmd == "Newgraph" && parts.size() > 1) {
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

bool Graph::eval(const vector<string>& parts) {
    const string& cmd = parts[0];

    if (cmd == "exit") {
        return false;
    } 
    if (cmd == "Newgraph") {
        int n = stoi(parts[1]);
        int m = stoi(parts[2]);
        //If Number of vertices of edges < 1 - exit program
        if (n < 1 || m < 1) return false;
        NewGraph(n, m);
        //If edges failed to complete - exit program
        if (!evalEdges(parts))
        {
            return false;
        }
    }
    else if (cmd == "Kosaraju") {
        vector<vector<int>> components = Kosaraju();
        //If Kosaraju failed - exit program
        if (components.empty()) return false;
        printOutput(components);
    } 
    else if (cmd == "Newedge") {
        int i = stoi(parts[1]);
        int j = stoi(parts[2]);
        //If edges wrong - exit program
        if (i > n || i < 1 || j > n || j < 1) return false;
        NewEdge(i, j);
    } 
    else if (cmd == "Removeedge") {
        int i = stoi(parts[1]);
        int j = stoi(parts[2]);
        //If edges wrong - exit program
        if (i > n || i < 1 || j > n || j < 1) return false;
        RemoveEdge(i, j);
    }
    
    return true;
}

bool Graph::evalEdges(const vector<string>& parts) {
    // Process each edge and call NewEdge
    // Split command edges based on ','
    for (size_t i = 2; i < parts.size(); ++i) {
        istringstream iss_edge(parts[i]);
        string edge;
        getline(iss_edge, edge, ',');
        int from = stoi(edge);
        getline(iss_edge, edge, ',');
        int to = stoi(edge);
        if (from > n || from < 1 || to > n || to < 1) return false;
        NewEdge(from, to);
    }
    return true;
}
