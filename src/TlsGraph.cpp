#include "../include/TlsGraph.h"
#include <cmath>
#include <iostream>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

TlsGraph::TlsGraph() {
    iterate_num_sampler = RandomRange{0.0,1.00};
    sqrt_m = sqrt(m);
    edge_sampler = RandomRange(0,2*m-1);
    from_vertexes.reserve(2*m);
    to_vertexes.reserve(2*m);
    edge_degs.reserve(2*m);
    for(int i=1;i<=n;i++){
        for(int j:con[i]){
            from_vertexes.push_back(i);
            to_vertexes.push_back(j);
        }
    }


    int cnt=0;
    for(int edge_cnt=0;edge_cnt<2*m;edge_cnt++){
        while (cnt++ % 10000000 == 0) cout<<"hash"<<endl;
        int left_node = from_vertexes[edge_cnt];
        int right_node = to_vertexes[edge_cnt];
        edge_degs.push_back(deg[left_node]+deg[right_node]);
    }
}

int TlsGraph::sample_wedge_based_edge(int edge) {
    return 0;
}

double TlsGraph::estimate_random_te() {
    int random_edge = edge_sampler.getRandomInt();
    int left_node=from_vertexes[random_edge],right_node=to_vertexes[random_edge];
    int left_deg=deg[left_node],right_deg=deg[right_node];
    RandomRange middle_node_sampler{0,left_deg+right_deg-1};
    int middle_node,isolate_node;
    if (middle_node_sampler.getRandomInt() < left_deg){
        middle_node = left_node;
        isolate_node = right_node;
    }else{
        middle_node = right_node;
        isolate_node = left_node;
    }
    int neighbor_node = get_random_neighbor(middle_node);
    if (neighbor_node == isolate_node) return 0;
    int low_deg_node,high_deg_node;
    if (deg[neighbor_node] < deg[isolate_node]){
        low_deg_node = neighbor_node;
        high_deg_node = isolate_node;
    }else{
        low_deg_node = isolate_node;
        high_deg_node = neighbor_node;
    }
    int iterate_num;
    if (deg[low_deg_node] < sqrt_m){
        if (iterate_num_sampler.getRandomReal()<deg[low_deg_node]/sqrt_m){
            iterate_num=1;
        }else{
            return 0;
        }
    }else {
        iterate_num = static_cast<int>(deg[low_deg_node] / sqrt_m) + 1;
    }
    vector<double> results;
    int last_node;
    for (int i=0;i<iterate_num;i++){
        double tmp=0;
        last_node = get_random_neighbor(low_deg_node);
        if (is_butterfly(last_node,middle_node,high_deg_node) && (last_node<low_deg_node)){
            tmp = max(sqrt_m,static_cast<double>(deg[low_deg_node]))*edge_degs[random_edge]/4.0;
        }
        results.push_back(tmp);
    }
    double result= accumulate(results.begin(),results.end(),0.0);
    return result/iterate_num;
}

double TlsGraph::tls_estimate(int time_limit_seconds) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto time_limit = std::chrono::seconds(time_limit_seconds);
    std::vector<double> results;
    while (1) {
        auto elapsed_time = std::chrono::high_resolution_clock::now() - start_time;
        if (elapsed_time >= time_limit) {
            break; // 如果已经超过时间上限，退出循环
        }
        double tmp = estimate_random_te();
        results.push_back(tmp);
    }

    cout << "num" << results.size()<<endl;
    double edge_average = std::accumulate(results.begin(), results.end(), 0.0)/results.size();
    // return edge_average*m;
    return edge_average*m;
}

int TlsGraph::get_random_neighbor(int vertex) {
    RandomRange sampler{0,deg[vertex]-1};
    return con[vertex][sampler.getRandomInt()];
}

bool TlsGraph::is_butterfly(int last_node, int middle_node, int high_deg_node) {
    if (last_node==middle_node) return false;
    auto it = lower_bound(con[high_deg_node].begin(),con[high_deg_node].end(),last_node);
    if (*it != last_node) return false;
    return true;
}
