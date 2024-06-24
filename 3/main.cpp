#include <iostream>
#include "kosa.hpp"

int main() {
    int n, m;
    std::cout << "Please insert the number of vertices in your graph: " << std::endl;
    std::cin >> n;
    std::cout << "Please insert the number of arcs in your graph: " << std::endl;
    std::cin >> m;
    
    std::vector<std::vector<int>> edges(m, std::vector<int>(2));
    std::cout << "enter the edges:" << std::endl;
    for (int i = 0; i < m; i++) {
        std::cout << "edge" << i+1 << ": " << std::endl;
        std::cin >> edges[i][0] >> edges[i][1];
        if (edges[i][0] > n || edges[i][1] > n)
        {
            std::cout << "ERROR: bad vertice! , try again" << std::endl;
            i--;
        }
        
    }

    Graph graph;
    graph.NewGraph(n, m, edges);
    std::vector<std::vector<int>> components = graph.Kosaraju();

    std::cout << "Output:" << std::endl;
    for (const auto& component : components) {
        for (int vertex : component) {
            std::cout << vertex << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}