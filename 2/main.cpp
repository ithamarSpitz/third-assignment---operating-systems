#include <iostream>
#include <vector>
#include <chrono>
#include "kosa.hpp"
#include "deque_Kosa.hpp"
#include "list_Kosa.hpp"

using namespace std;
using namespace std::chrono;

template<typename T>
void testGraph(T& graph, int n, int m, const vector<vector<int>>& edges) {
    auto start = high_resolution_clock::now();

    graph.NewGraph(n, m, edges);
    auto components = graph.Kosaraju();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);

    cout << "Strongly Connected Components:" << endl;
    for (const auto& comp : components) {
        for (int vertex : comp) {
            cout << vertex << " ";
        }
        cout << endl;
    }

    cout << "Time taken: " << duration.count() << " seconds\n\n";
}

template<typename T>
void testGraph_Q(T& graph_deque, int n, int m, const vector<deque<int>>& edges) {
    auto start = high_resolution_clock::now();

    graph_deque.NewGraph(n, m, edges);
    auto components = graph_deque.Kosaraju();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);

    cout << "Strongly Connected Components:" << endl;
    for (const auto& comp : components) {
        for (int vertex : comp) {
            cout << vertex << " ";
        }
        cout << endl;
    }

    cout << "Time taken: " << duration.count() << " seconds\n\n";
}


template<typename T>
void testGraph_L(T& graph_list, int n, int m, const vector<list<int>>& edges) {
    auto start = high_resolution_clock::now();

    graph_list.NewGraph(n, m, edges);
    auto components = graph_list.Kosaraju();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);

    cout << "Strongly Connected Components:" << endl;
    for (const auto& comp : components) {
        for (int vertex : comp) {
            cout << vertex << " ";
        }
        cout << endl;
    }

    cout << "Time taken: " << duration.count() << " seconds\n\n";
}

int main() {
    int n = 5;
    int m = 7;

    vector<vector<int>> edges = {
        {1, 2},
        {2, 3},
        {3, 1},
        {3, 4},
        {4, 5},
        {5, 4},
        {2, 5}
    };

    Graph graph;
    Graph_deque graph_deque;
    Graph_list graph_list;

    // cout << "Testing with std::vector<int>:\n";
    // testGraph(graph, n, m, edges);
    

    vector<deque<int>> edges_Q = {
        {1, 2},
        {2, 3},
        {3, 1},
        {3, 4},
        {4, 5},
        {5, 4},
        {2, 5}
    };

    graph_deque.NewGraph(n, m, edges_Q);
    std::vector<std::deque<int>> components1 = graph_deque.Kosaraju();

    // cout << "Testing with std::deque<int>:\n";
    // testGraph_Q(graph_deque, n, m, edges_Q);

    // vector<list<int>> edges_L = {
    //     {1, 2},
    //     {2, 3},
    //     {3, 1},
    //     {3, 4},
    //     {4, 5},
    //     {5, 4},
    //     {2, 5}
    // };

    // graph_list.NewGraph(n, m, edges_L);
    // std::vector<std::list<int>> components2 = graph_list.Kosaraju();

    // cout << "Testing with std::list<int>:\n";
    // testGraph_L(graph_list, n, m, edges_L);

    return 0;
}
