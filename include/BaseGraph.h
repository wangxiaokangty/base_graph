#ifndef BASE_GRAPH_BASEGRAPH_H
#define BASE_GRAPH_BASEGRAPH_H

#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

extern std::unordered_map<std::string, std::string> config;
extern std::unordered_map<std::string, std::string> data_config;
// An undirected graph will be converted into a directed graph.
class BaseGraph {
public:
    int n_left,n_right,m,n;
    vector<vector<int>> con;
    vector<int> deg;

public:
    BaseGraph();
    void save_bin();
    void read_bin();

};


#endif
