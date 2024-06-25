#include <iostream>
#include "kosa.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <vertices> <edges>" << endl;
        return 1;
    }

    int n = atoi(argv[1]); // Number of vertices
    int m = atoi(argv[2]); // Number of edges

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
    graph.printOutput(components);
    return 0;
}