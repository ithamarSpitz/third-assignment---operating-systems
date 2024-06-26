#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "kosa.hpp"

using namespace std;

vector<string> parse(const string& command);
bool eval(Graph& graph, const vector<string>& parts);

int main() {
    Graph graph;
    string command;

    while (getline(cin, command)) {
        vector<string> parts = parse(command);
        if (!parts.empty()) {
            if (!eval(graph, parts)) {
                break;
            }
        }
    }

    return 0;
}

vector<string> parse(const string& command) {
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

bool eval(Graph& graph, const vector<string>& parts) {
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
