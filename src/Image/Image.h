//
// Created by notpyxl on 18/07/20.
//

#ifndef CS3102_GNG_FSOCUALAYA_IMAGE_H
#define CS3102_GNG_FSOCUALAYA_IMAGE_H

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include "../Graph/Graph.h"

typedef std::vector<int> intVector ;
typedef std::vector<cv::Point> pointVector;
typedef std::vector<intVector> intMatrix;

#define THRESHOLD 150

#define ON 255
#define OFF 0
#define GREEN cv::Scalar(OFF, ON, OFF)
#define BLUE cv::Scalar(ON, OFF, OFF)

#define W1_FACTOR 0.05
#define WN_FACTOR 0.0006
#define MAX_AGE 100
#define LAMBDA 300
#define MAX_NODE_COUNT 20

#define ALPHA 0.5
#define BETA 0.0005

class Image {
    cv::Mat content;
public:

    void readImage(const std::string&);
    Image() = default;
    explicit Image(const std::string&);
    explicit Image(const intMatrix&);
    void displayImage();
    void write(std::string filename, const int& iteration, const cv::Mat& image);

    Image GNG();
    Image canny();
    Image sobel();

    void drawCircle(const cv::Point& point);
    void drawLine(const cv::Point& src, const cv::Point& dst);
    void plotGraph(Graph&);

    intMatrix getBinaryMatrix();
    pointVector getBoundaries();
};


#endif //CS3102_GNG_FSOCUALAYA_IMAGE_H
