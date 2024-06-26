#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "kosa.hpp"

using namespace std;

int main() {
    Graph graph;
    string command;

    while (getline(cin, command)) {
        vector<string> parts = graph.parse(command);
        if (!parts.empty()) {
            if (!graph.eval(graph, parts)) {
                break;
            }
        }
    }

    return 0;
}