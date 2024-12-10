#include "../include/TlsGraph.h"
#include <cmath>
#include <iostream>

using namespace std;

TlsGraph::TlsGraph() {
    sqrt_m = sqrt(m);
    edge_sampler = RandomRange(0,2*m-1);
    from_vertexes.reserve(2*m);
    to_vertexes.reserve(2*m);
    vertex_degs.reserve(n);
    edge_degs.reserve(2*m);
    vertex_degs.push_back(0); // for index 0
    for(int i=1;i<=n;i++){
        vertex_degs.push_back(con[i].size());
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
        is_edge.insert({left_node,right_node});
        int left = vertex_degs[left_node];
        int right = vertex_degs[right_node];
        edge_degs.push_back(left+right);
    }
}

int TlsGraph::sample_wedge_based_edge(int edge) {
    return 0;
}

double TlsGraph::estimate_random_te() {
    int random_edge =  edge_sampler.getRandomInt();
    int left_node=from_vertexes[random_edge],right_node=to_vertexes[random_edge];
    int left_node_deg = vertex_degs[left_node],right_node_deg = vertex_degs[right_node];
    if (left_node_deg<2||right_node_deg<2){
        return 0;
    }
    RandomRange wedge_sapler{0,left_node_deg+right_node_deg-1};
    int wedge_random = wedge_sapler.getRandomInt();
    if(wedge_random<left_node_deg){
        // left_node in a side , the others in another side
        int new_node = con[left_node][wedge_random];
        if(new_node==right_node) return 0;
        double bias = static_cast<double>(left_node_deg+right_node_deg) / (left_node_deg+right_node_deg - 2);
        if(right_node_deg<vertex_degs[new_node]){
            int tmp = new_node;
            new_node = right_node;
            right_node = tmp;
        }
        // new_node is the node with lower degree than right node
        int iterate_num;
        if(vertex_degs[new_node]<sqrt_m){
            RandomRange real_sampler{0.0,1.0};
            if(real_sampler.getRandomReal() < static_cast<double>(vertex_degs[new_node])/sqrt_m){
                iterate_num = 1;
            }else {
                return 0;
            }
        }else {
            iterate_num = static_cast<int>(vertex_degs[new_node]/sqrt_m)+1;
        }
        // iterate_num != 1,loop will run
        std::vector<double> results;
        RandomRange neighbor_sampler{0,vertex_degs[new_node]-1};
        int number = neighbor_sampler.getRandomInt();
        int fourth_node = con[new_node][number];
        for(int i=0;i<iterate_num;i++){
            double result = 0;
            if ( (fourth_node<new_node) && (fourth_node!=left_node) && (is_edge.find({fourth_node,right_node})!=is_edge.end())){
                result = std::max(static_cast<double>(vertex_degs[new_node]),sqrt_m)*0.25;
            }
            results.push_back(result);
        }
        double sum = static_cast<double>(std::accumulate(results.begin(), results.end(), 0.0));
        return sum*edge_degs[random_edge]/iterate_num;
    }else {
        // right_node in a side , the others in another side
        int new_node = con[right_node][wedge_random-left_node_deg];
        if(new_node==left_node) return 0;
        double bias = static_cast<double>(left_node_deg+right_node_deg) / (left_node_deg+right_node_deg - 2);
        if(left_node_deg<vertex_degs[new_node]){
            int tmp = new_node;
            new_node = left_node;
            left_node = tmp;
        }
        // new_node is the node with lower degree than left node
        int iterate_num;
        if(vertex_degs[new_node]<sqrt_m){
            RandomRange real_sampler{0.0,1.0};
            if(real_sampler.getRandomReal() < static_cast<double>(vertex_degs[new_node])/sqrt_m){
                iterate_num = 1;
            }else {
                return 0;
            }
        }else {
            iterate_num = static_cast<int>(vertex_degs[new_node]/sqrt_m)+1;
        }
        // iterate_num != 1,loop will run
        std::vector<double> results;
        RandomRange neighbor_sampler{0,vertex_degs[new_node]-1};
        int number = neighbor_sampler.getRandomInt();
        int fourth_node = con[new_node][number];
        for(int i=0;i<iterate_num;i++){
            double result = 0;
            if ( (fourth_node<new_node) && (fourth_node!=right_node) && (is_edge.find({fourth_node,left_node})!=is_edge.end())){
                result = std::max(static_cast<double>(vertex_degs[new_node]),sqrt_m)*0.25;
            }
            results.push_back(result);
        }
        double sum = static_cast<double>(std::accumulate(results.begin(), results.end(), 0.0));
        return sum*edge_degs[random_edge]/iterate_num;
    }
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

    double edge_average = std::accumulate(results.begin(), results.end(), 0.0)/results.size();
    // return edge_average*m;
    return edge_average*m;
}
