cmake_minimum_required(VERSION 3.15)
project(cs3102_gng_fsocualaya)

set(CMAKE_CXX_STANDARD 14)

find_package( OpenCV REQUIRED )

add_executable(cs3102_gng_fsocualaya
        src/main.cpp
        src/Image/Image.cpp
        src/Graph/Graph.cpp
        )

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
