#include <iostream>
#include <vector>
#include <chrono>
#include "kosa.hpp"
#include "deque_Kosa.hpp"
#include "list_Kosa.hpp"

using namespace std;
using namespace std::chrono;


template<typename GraphType>
void testGraph(GraphType& graph, int n, int m, const vector<vector<int>>& edges) {
    auto start = high_resolution_clock::now();

    graph.NewGraph(n, m, edges);
    auto components = graph.Kosaraju();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Strongly Connected Components:" << endl;
    graph.printOutput(components);

    cout << "Time taken: " << duration.count() << " microseconds\n\n";
}


int main() {
    int n = 5;
    int m = 5;

    vector<vector<int>> edges = {
        {1, 2},
        {2, 3},
        {3, 1},
        {3, 4},
        {4, 5},
    };

    // // very large random graph
    // int n = 1000; 
    // int m = 5000; 

    // vector<vector<int>> edges;
    // for (int i = 0; i < m; ++i) {
    //     edges.push_back({rand() % n + 1, rand() % n + 1});
    // }

    

    Graph graph;
    Graph_deque graph_deque;
    Graph_list graph_list;

    cout << "Testing with std::vector<int>:\n";
    testGraph(graph, n, m, edges);
    


    cout << "Testing with std::deque<int>:\n";
    testGraph(graph_deque, n, m, edges);



    cout << "Testing with std::list<int>:\n";
    testGraph(graph_list, n, m, edges);

    return 0;
}

/*TEST_RUNS:
    graph.NewGraph(n, m, edges);
    std::vector<std::vector<int>> components = graph.Kosaraju();
    graph.printOutput(components);


    graph_list.NewGraph(n, m, edges);
    components = graph_list.Kosaraju();
    graph_list.printOutput(components);

    graph_deque.NewGraph(n, m, edges);
    components = graph_deque.Kosaraju();
    graph_deque.printOutput(components);

*/
