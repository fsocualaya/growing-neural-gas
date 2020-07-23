//
// Created by notpyxl on 20/07/20.
//

#ifndef CS3102_GNG_FSOCUALAYA_GRAPH_H
#define CS3102_GNG_FSOCUALAYA_GRAPH_H

#include "Node.h"
#include <vector>

class Graph {
public:
    std::vector<Node*> adj_list;
    Graph();
    int inserted_nodes;
    void addEdge(int src_idx, int dst_idx);
    void removeEdge(int src_idx, int dst_idx);
    void addNode();
    void removeVertex(int idx);
    Node* getNode(int idx);
    std::pair<Node*, Node*> getWinners(const cv::Point& p);
    void print();
};


#endif //CS3102_GNG_FSOCUALAYA_GRAPH_H
