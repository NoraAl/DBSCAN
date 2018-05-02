#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <iostream>
#include <vector>
#include <iomanip>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"


#define RESET   "\033[0m"
#define BRED     "\033[1m\033[31m"      /* Bold Red */
#define BGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BCYAN    "\033[1m\033[36m"      /* Bold Cyan */


using namespace std;
using namespace cv;

struct P {
    P(double x, double y) : x(x), y(y), neighbors(0), cluster(-1),core(false), boundary(false), noise(false){}

    double x;
    double y;
    int neighbors;
    int cluster; // zero cluster means not clustered yet
    bool core;
    bool boundary;
    bool noise;
};

class Colors{
public:
    Colors();
    ~Colors();
    Scalar color(int index);
    Scalar boundary(int index);
    Scalar noise();
    Scalar shadow();
    Scalar operator[](int);
private:
    vector<Scalar> colors;
};

typedef vector<P> Points;

bool operator==(const P &p1, const P &p2);

bool operator!=(const P &p1, const P &p2);

Points readPoints(string filename, bool centroids, char separator = ',');

void generateRandom(Points &points, double minX, double maxX, int num, double minY=-1, double maxY=-1);

void cluster(int minPts, bool show = false);

void plot(Points points, Mat image, int minPts, double epsilon, bool last);

void plotPath(Points points, Mat image, int i, int j);

void printPoints(Points points) ;

double getDistance(P p1, P p2);


#endif