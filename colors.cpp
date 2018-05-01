#include "cluster.hpp"

Colors::Colors() {
    colors.emplace_back(255, 255, 255);//white
    colors.emplace_back(102, 255, 255);//3
    colors.emplace_back(204, 102, 255);//6
    colors.emplace_back(0, 0, 255);//red for centroids
    colors.emplace_back(0,255,0);//green
    colors.emplace_back(255, 30, 127);//9
    colors.emplace_back(0, 153, 255);//orange
    colors.emplace_back(255, 255, 0);//1
    colors.emplace_back(255, 255, 0);//yellow --shadow--
    colors.emplace_back(200,200,200);//gray for noise

    colors.emplace_back(255, 255, 255);//red for centroids
}

Colors::~Colors() {
    //automatic deallocation
}

Scalar Colors::color(int index) {
    return colors[index];
}

Scalar Colors::boundary(int index) {
    return colors[index];
}

Scalar Colors::noise() {
    return colors[colors.size()-2];
}

Scalar Colors::shadow() {
    return colors[colors.size()-1];
}

Scalar Colors::operator[](const int index){
    return colors[index];
}
