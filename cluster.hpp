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

enum MEASURE {
    Euclidean, Manhattan
};

// Point structure, "Point" is reserved for one of cv types
struct P {
    P(double x, double y) : x(x), y(y), neighbors(0), cluster(0),core(false), boundary(false), noise(false){}

    double x;
    double y;
    int neighbors;
    int cluster; // zero cluster means not clustered yet
    bool core;
    bool boundary;
    bool noise;
};
typedef vector<P> Points;

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

// overload equal to check point equality
bool operator==(const P &p1, const P &p2);

bool operator!=(const P &p1, const P &p2);

Points readPoints(string filename, bool centroids, char separator = ',');

Points  generateRandom(Points &points, double minX, double maxX, int num, bool centroids = false);

Points  generateRandom(Points &points, double minX, double maxX, int num, bool centroids, double minY, double maxY);

Points initialCenters(Points centroids, double minP, double maxP, int k);

void cluster(int minPts, MEASURE measure = Euclidean);

void plot(Points points);

void printPoints(Points points) ;

double getDistance(P p1, P p2, MEASURE m);

inline double getEuclidean(P p1, P p2);//Euclidean

inline double getManhattan(P p1, P p2);//Manhattan

void intracluster(MEASURE m, int k);

void minMax(Points &points, Points &centroids, MEASURE m);

string fnum(double num);


#endif