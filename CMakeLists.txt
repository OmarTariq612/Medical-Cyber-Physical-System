cmake_minimum_required(VERSION 3.0)

project(MyHeart)

set(CMAKE_CXX_STANDARD 20)

set(SRC src/main.cpp src/node_model.cpp src/path_model.cpp src/heart_model.cpp)
set(INC inc/node_model.hpp inc/path_model.hpp inc/heart_model.hpp)

include_directories(INC)

add_executable(MyHeart ${SRC})
