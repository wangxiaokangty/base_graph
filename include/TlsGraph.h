#ifndef BASE_GRAPH_TLSGRAPH_H
#define BASE_GRAPH_TLSGRAPH_H

#include <unordered_set>

#include "BaseGraph.h"
#include "RandomRange.h"


using namespace std;

namespace std{
    template<> struct hash<pair<int, int>>{
        size_t operator()(const pair<int, int> &p) const {
            auto res = (static_cast<size_t>(p.first) << (sizeof(int) * 8)) + static_cast<size_t>(p.second);
            return res;
        }
    };
    template<> struct equal_to<pair<int, int>>{
        bool operator()(const pair<int, int> &p1, const pair<int, int> &p2) const {
            return p1.first == p2.first && p1.second == p2.second;
        }
    };
}

class TlsGraph : BaseGraph {
public:
    std::unordered_set<std::pair<int, int>> is_edge;
    std::vector<int> from_vertexes,to_vertexes,vertex_degs,edge_degs;
    RandomRange edge_sampler,iterate_num_sampler;
    double sqrt_m;

public:
    TlsGraph();
    int sample_wedge_based_edge(int edge);
    double tls_estimate(int time_limit_seconds);
    double estimate_random_te();
    int get_random_neighbor(int vertex);
    bool is_butterfly(int last_node,int middle_node,int isolate_node);

};


#endif //BASE_GRAPH_TLSGRAPH_H
