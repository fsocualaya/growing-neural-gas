//
// Created by notpyxl on 18/07/20.
//

#include <iostream>
#include "Image.h"

void Image::readImage(const std::string& filename) {

    content = cv::imread(filename);
    if(!content.data) {
        std::cerr << "An error occurred while opening the file\n";
        exit(EXIT_FAILURE);
    }
}

Image::Image(const std::string& filename) {
    readImage(filename);
}

void Image::displayImage() {
    std::string window_name = "GNG";
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    cv::imshow(window_name, content);
    cv::waitKey(0);
}

Image Image::GNG() {

    srand(time(nullptr));

    Image gng = *this;
    gng.content = content;
    Graph network;
    network.addNode();
    network.addNode();
    network.getNode(0)->coords = {rand() % content.cols,
                                  rand()%content.rows };
    network.getNode(1)->coords = {rand() % content.cols,
                                  rand()%content.rows};

    // Iterates over all the bounds
    int signals_generated = 0;
    auto boundaries = getBoundaries();
    do{ // TODO: Change stop criteria and the way to get
        std::cout<<"Iteration: "<< signals_generated<< " Nodes: "<< network.adj_list.size()<<'\n';
        // Iterates over all the current nodes to find the winner
        cv::Point bound = boundaries[rand() % boundaries.size()];
        auto winners = network.getWinners(bound);
        // printf("Winners:\t w1=[%d, %d]\t w2=[%d, %d]\n",
        // winners.first.coords.x, winners.first.coords.y,
        // winners.second.coords.x, winners.second.coords.y);

        // Update local error variable for w1
        network.getNode(winners.first->index)->error += std::pow(winners.first->getDistance(bound), 2);

        // Move winner and neighbours towards the current point
        network.getNode(winners.first->index)->move_towards(bound, W1_FACTOR);
        for(auto&neighbour:network.getNode(winners.first->index)->edges )
            network.getNode(neighbour.first->index)->move_towards(bound, WN_FACTOR);

        // Update age for all w1's neighbours
        for(auto&neighbour:network.getNode(winners.first->index)->edges)
            neighbour.second++;

        network.addEdge(winners.first->index, winners.second->index);

        // Remove old nodes
        for(auto&node:network.adj_list) {
            for (auto &edge:node->edges) {
                if (edge.second > MAX_AGE)
                    network.removeEdge(node->index, edge.first->index);
            }
            if(node->edges.empty())
                network.removeVertex(node->index);
        }

        // Add node to the graph
        if(signals_generated % LAMBDA == 0 && network.adj_list.size() < MAX_NODE_COUNT) {
            auto larger_error_node = *network.adj_list.begin() ;
            for (auto &node:network.adj_list) {
                if (node->error > larger_error_node->error)
                    larger_error_node = node;
            }

            auto larger_error_neighbour = (*larger_error_node->edges.begin()).first;
            for (auto &node:larger_error_node->edges)
                if (node.first->error > larger_error_neighbour->error)
                    larger_error_neighbour = node.first;

            network.addNode();
            // Get Last inserted node and set its position to the middle of
            // previous computed nodes

            auto new_node = network.adj_list[network.adj_list.size() - 1]; // TODO: Fix if problems
            new_node->coords.x = (larger_error_node->coords.x +
                                  larger_error_neighbour->coords.x) / 2.0;
            new_node->coords.y = (larger_error_node->coords.y +
                                  larger_error_neighbour->coords.y) / 2.0;

            // Connect the new node with both of the previous computed nodes
            network.addEdge(new_node->index, larger_error_node->index);
            network.addEdge(new_node->index, larger_error_neighbour->index);

            // Disconnect previously computed nodes
            network.removeEdge(larger_error_node->index, larger_error_neighbour->index);

            std::cout<<"Updating errors\n";
            network.getNode(larger_error_node->index)->error *= ALPHA;
            network.getNode(larger_error_neighbour->index)->error *= ALPHA;

            new_node->error = larger_error_node->error;

            // gng = *this;
            // gng.plotGraph(network);
            // gng.write("utec_1.jpg",signals_generated, gng.content);
        }
        for(auto&node:network.adj_list) {
            node->error -= BETA * node->error;
        }

        signals_generated++;
    }while(signals_generated <= 5 * boundaries.size());

    network.print();
    plotGraph(network);
    return gng;
}

Image Image::canny() {
    Image edges;
    cv::Canny(content, edges.content,70, 200);
    cv::cvtColor(edges.content, edges.content, cv::COLOR_GRAY2BGR);
    return edges;
}

Image Image::sobel(){
    cv::Mat gray;
    cv::cvtColor(content, gray, cv::COLOR_BGR2GRAY);

    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

    cv::Sobel(gray, grad_x, CV_16S, 1, 0, 3, 1, 0 , cv::BORDER_DEFAULT);
    cv::Sobel(gray, grad_y, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT);

    convertScaleAbs( grad_x, abs_grad_x );
    convertScaleAbs( grad_y, abs_grad_y );

    cv::Mat grad;
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    cv::cvtColor(grad, grad, cv::COLOR_GRAY2BGR);

    Image sobel_img;
    sobel_img.content = grad;
    return sobel_img;
}

intMatrix Image::getBinaryMatrix(){
    cv::Mat tmp_gray_img = content;
    cv::cvtColor(tmp_gray_img, tmp_gray_img, cv::COLOR_BGR2GRAY);
    intMatrix binary_matrix(tmp_gray_img.rows, std::vector<int>(tmp_gray_img.cols));
    for(int i = 0; i<tmp_gray_img.rows;++i){
        for(int j=0; j<tmp_gray_img.cols;++j){
            binary_matrix[i][j] = (tmp_gray_img.at<uchar>(i, j) > THRESHOLD) ? ON : OFF;
        }
    }
    return binary_matrix;
}

Image::Image(const intMatrix& matrix){
    int rows = matrix.size(), cols = matrix[0].size();
    cv::Mat img(cv::Size(cols, rows),CV_8UC(1),cv::Scalar::all(0));
    for(int i=0;i<rows;++i){
        for(int j = 0; j<cols ;++j){
            img.at<uchar>(i, j) = matrix[i][j];
        }
    }
    cv::cvtColor(img, img, cv::COLOR_GRAY2BGR);
    this->content = img;
}

void Image::drawCircle(const cv::Point& point) {
    cv::circle(this->content, point, 1, GREEN, 1);
}

pointVector Image::getBoundaries() {
    Image edges(this->sobel().getBinaryMatrix());
    // Fix using the binary matrix
    cv::cvtColor(edges.content, edges.content, cv::COLOR_BGR2GRAY);
    pointVector boundaries;
    for(int i = 0; i<edges.content.rows;++i){
        for(int j=0; j<edges.content.cols;++j){
            if(edges.content.at<uchar>(i,j)==ON)
                boundaries.push_back(cv::Point(j,i));
        }
    }
    return boundaries;
}

void Image::plotGraph(Graph& graph) {
    for(auto&node:graph.adj_list){
        for(auto&neighbour:node->edges)
            drawLine(node->coords, neighbour.first->coords);
        drawCircle(node->coords);
    }
}

void Image::drawLine(const cv::Point &src,const cv::Point& dst) {
    cv::line(this->content, src, dst, BLUE, 1);
}

void Image::write(std::string filename,  const int &iteration, const cv::Mat& image) {
    std::string path = "../results/"+ std::to_string(iteration)  + filename;
    cv::imwrite(path, image);
}

