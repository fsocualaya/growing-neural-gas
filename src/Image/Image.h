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

#define THRESHOLD 250

#define ON 255
#define OFF 0
#define GREEN cv::Scalar(OFF, ON, OFF)

#define BLACK cv::Scalar(OFF, OFF, OFF)

#define W1_FACTOR 0.0005
#define WN_FACTOR 0.0003
#define MAX_AGE 20
#define LAMBDA 5
#define MAX_NODE_COUNT 10000

#define ALPHA 0.01
#define BETA 0.02

class Image {
    cv::Mat content;
public:

    void readImage(const std::string&);
    Image() = default;
    explicit Image(const std::string&);
    explicit Image(const intMatrix&);
    void displayImage();

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
