//
// Created by notpyxl on 20/07/20.
//

#ifndef CS3102_GNG_FSOCUALAYA_NODE_H
#define CS3102_GNG_FSOCUALAYA_NODE_H

#include <list>
#include <set>
#include <opencv2/core/types.hpp>
#include <map>

struct Node{
    double error;
    int index;

    bool operator==(const Node* node) const
    {
        return (this == node);
    }

    bool operator==(const Node& node) const
    {
        return (this->index == node.index);
    }

    bool operator<(const Node& node) const{
        return this->index < node.index;
    };

    std::map<Node*, int> edges; // on the map: first(dest), second(age)
    cv::Point coords;

    void initNode(){
        error = 0;
        coords = {0, 0};
    }

    Node(){
        initNode();
        index = 0;
    };
    Node(int idx){
        initNode();
        index = idx;
    };

    double getDistance(const cv::Point& p){
        return sqrt(std::pow(p.x - coords.x, 2) +
                    std::pow(p.y - coords.y, 2));
    }

    void move_towards(const cv::Point& p, double factor){
        double step_x, step_y;
        step_x = factor*getDistance({p.x, 0});
        step_y = factor*getDistance({0, p.y});

        if(p.x < this->coords.x)
            step_x*=-1;
        if(p.y < this->coords.y)
            step_y*=-1;

        this->coords.x += round(step_x);
        this->coords.y += round(step_y);
    }
};

#endif //CS3102_GNG_FSOCUALAYA_NODE_H
