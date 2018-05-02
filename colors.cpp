#include "cluster.hpp"

Colors::Colors() {
    colors.emplace_back(255, 0, 255);//purple pink
    colors.emplace_back(0, 0, 255);//red
    colors.emplace_back(0,255,0);//neon green
    colors.emplace_back(255, 0, 0);//neon blue
    colors.emplace_back(0, 153, 255);//orange
    colors.emplace_back(153, 204, 0); //ter
    colors.emplace_back(255, 255, 0);//yellow
    colors.emplace_back(255, 128, 128);//blue
    colors.emplace_back(0, 204, 153); //green


    colors.emplace_back(190,190,190);//gray for noise
    colors.emplace_back(255, 255, 255);//white for the shadow
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
