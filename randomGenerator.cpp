#include "cluster.hpp"

/****************************
 *
 *
 * generate random points
 *
 *
 ****************************/
inline int getRandom(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void generateRandom(Points &points, double minX, double maxX, int num, double minY, double maxY) {
    minY = (minY<0)? minX:minY;
    maxY = (maxY<0)? maxX:maxY;

    P point = P(0,0);
    while (num) {
        point.x = getRandom(minX, maxX);
        point.y = getRandom(minY, maxY);

        points.push_back(point);
        num--;
    }
}
