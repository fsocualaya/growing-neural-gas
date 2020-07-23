//
// Created by notpyxl on 20/07/20.
//

#include <iostream>
#include "Graph.h"

Graph::Graph() {
    inserted_nodes = 0;
}

void Graph::addNode() {
    // TODO: Modify function (Can fall on duplicate indexes)
    // Possible solution: add a variable that stores the number of total insertions
    Node* node = new Node(inserted_nodes);
    adj_list.push_back(node);
    inserted_nodes++;
}

void Graph::addEdge(int src_idx, int dst_idx) {
    auto src_exist = getNode(src_idx);
    auto dst_exist = getNode(dst_idx);
    if(src_exist && dst_exist){
        /*
        auto it1 = src_exist->edges.find(dst_exist);
        auto it2 = dst_exist->edges.find(src_exist);

        if(it1 != src_exist->edges.end() and
           it2 != dst_exist->edges.end()) {
            it1->second = 0;
            it2->second = 0;
        }else {
            src_exist->edges[dst_exist] = 0;
            dst_exist->edges[src_exist] = 0;
        }*/
        src_exist->edges[dst_exist] = 0;
        dst_exist->edges[src_exist] = 0;
    }
}

void Graph::removeEdge(int src_idx, int dst_idx) {
    auto src_exist = getNode(src_idx);
    auto dst_exist = getNode(dst_idx);

    if(src_exist && dst_exist) {
        auto it1 = src_exist->edges.find(dst_exist);
        auto it2 = dst_exist->edges.find(src_exist);

        if(it1 != src_exist->edges.end() and
            it2 != dst_exist->edges.end()) {
            src_exist->edges.erase(it1);
            dst_exist->edges.erase(it2);
        } else {
            std::cerr << "Index not found. Couldn't remove edge\n";
            exit(0);
        }
    } else {
        std::cerr << "Index not found. Couldn't remove edge\n";
        //exit(0);
    }
}

void Graph::removeVertex(int idx) {
    auto it = std::find(adj_list.begin(), adj_list.end(), getNode(idx));

    for(auto&node:adj_list){
        auto iter = node->edges.find(getNode(idx));
        if(iter!=node->edges.end())
            node->edges.erase(iter);
    }

    adj_list.erase(it);

    std::cerr<< "Removed Vertex " << idx<<'\n';
}

void Graph::print() {
    for(auto&node:adj_list){
        std::cout<<node->index<<" -> ";
        for(auto&neighbour:node->edges){
            std::cout<<neighbour.first->index<<" ["
            <<neighbour.first->coords.x<<", "
            << neighbour.first->coords.y<<"] ";
        }
        std::cout<<'\n';
    }
}

Node* Graph::getNode(int idx) {
    // TODO: Look for a way to sort by index to compute faster this operation
    for(auto&node:adj_list)
        if(node->index == idx)
            return node;
    std::cerr<<"Node "<< idx << "does not exist\n";
    return nullptr;
}

std::pair<Node*, Node*> Graph::getWinners(const cv::Point& p) {
    if(!adj_list.empty()) {
        std::pair<Node*, Node*> winners;
        std::pair<double, double> distances = {INFINITY, INFINITY};
        double tmp_distance;
        for (auto&node:adj_list){
            tmp_distance = node->getDistance(p);
            if(tmp_distance < distances.first) {
                // Update distances
                distances.second = distances.first;
                distances.first = tmp_distance;
                // Update winners;
                winners.second = winners.first;
                winners.first = node;
            }else if ( tmp_distance < distances.second)
                winners.second = node;
        }
        return winners;
    }else{
        std::cerr<<"No enough vertices to compute winners.\n";
        exit(0);
    }
}