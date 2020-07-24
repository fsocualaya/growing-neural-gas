#include <iostream>
#include "Image/Image.h"
#include "Graph/Graph.h"

int main() {
    Image image("../img/fist.jpg");

    // Image edges(image.canny().getBinaryMatrix());

    // Image gng = image.canny().GNG();
    // image.sobel().displayImage();
    image.sobel().GNG().displayImage();
    return 0;
}