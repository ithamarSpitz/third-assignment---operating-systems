#include <vector>
#include <string>
using namespace std;


// you should add a global class/variable of the graph itself

//creates new graph, returns -1 if fail
int NewGraph(int n, int m, vector<vector<int>> edges){}

//adds new edge, returns -1 if fail
int NewEdge(int i, int j){}

//remove specific edge, returns -1 if fail
int RemoveEdge(int i, int j){}

//returns vector of Strongly Connected Components vectors
vector<vector<int>> Kosaraju(){}

//parsethe user's input, returns array of variables (the first is the desired function or -1 if failed)
vector<int> parse(string input){}

//using parse's output run the desired function
int eval(vector<int>){}

