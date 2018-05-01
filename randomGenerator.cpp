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

Points generateRandom(Points &points, double minX, double maxX, int num, bool centroids, double minY, double maxY) {
    P point = P(0,0);

    while (num) {
        point.x = getRandom(minX, maxX);
        point.y = getRandom(minY, maxY);

        points.push_back(point);
        num--;
    }

    return points;
}

Points generateRandom(Points &points, double minX, double maxX, int num, bool centroids) {
    return generateRandom(points, minX, maxX, num, centroids, minX, maxX);
}

/****************************
 *
 *
 * initiate centroids
 *
 *
 ****************************/

Points initialCenters(Points centroids, double minP, double maxP, int k) {
    generateRandom(centroids, minP, maxP, k, true);
    Points p = centroids;
    // make sure that initial centroids are unique
    auto fun = [&](int index) {
        int ii = centroids.size();
        for (int i = index; i < (centroids.size() - 1); i++) {
            //todo: compare new generated from beginning if such
            for (int j = i + 1; j < centroids.size(); j++) {
                if (centroids[i].x == centroids[j].x)
                    if (centroids[i].y == centroids[j].y)
                        return i;
            }
        }
        return -1;
    };

    int index = fun(0);

    while (index >= 0) {
        index = fun(index);
        centroids[index].x = getRandom(1, 100);
        centroids[index].y = getRandom(1, 100);
    }

    return centroids;
}
