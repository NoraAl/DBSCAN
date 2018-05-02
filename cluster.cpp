#include "cluster.hpp"

static Points points;
static int size;
static double epsilon = 2;

/****************************
 *
 *
 * main
 *
 *
 ****************************/
bool lessThan(P i, P j) {
    if (i.x < j.x)
        return true;
    if (i.x == j.x)
        return i.y < j.y;
    return false;
}

int main() {
    try {

        srand(time(nullptr));

        vector<int> pCount = {20,60,100};//212};//{30, 20, 1000};
        vector<int> minPoints = {3,5}; //todo: accept also from user
        vector<double>epsilons = {2,10,20};


        for (auto i: pCount) {// for each file with variant number of points
            for (auto ep: epsilons) { // for variant minPoints
                for (auto minPts: minPoints){string pointfile = format("../points_%d.txt", i);
                    epsilon = ep;
                    cout << BCYAN << "\n**************\nMinPts = " << minPts << "\nEpsilon = " << epsilon << RESET << endl;

                    points = readPoints(pointfile, false);

                    size = points.size();

                    sort(points.begin(), points.end(), lessThan);
                    printPoints(points);
                    cluster(minPts);}
            }
        }
        return 0;
    } catch (const char *error) {
        cout << "\nError: " << error << endl;
        return 1;
    }

}


Mat createImage() {
    Mat image = Mat::zeros(620, 620, CV_8UC3);
    int padding = 20, size = 600, windowSize = size + padding;
    int scale = 4;
    int p2 = padding / 2;

    return image;
}


/****************************
 *
 *
 * DBSCAN algorithm
 *
 *
 ****************************/
void cluster(int minPts, bool show) {

    Mat image;
    // create path matrix and fill it with zeros
    bool paths[size][size];
    for (int i = 0; i < points.size(); ++i) {
        for (int j = 0; j < points.size(); ++j) {
            paths[i][j] = false;
        }
    }

    // find connected pairs and core points
    for (int i = 0; i < points.size(); ++i) {
        for (int j = i + 1; j < points.size(); ++j) {
            if (getDistance(points[i], points[j]) <= epsilon) {
                points[i].neighbors++;
                points[j].neighbors++;
                paths[i][j] = true;
                paths[j][i] = true;


            }
        }
        if (points[i].neighbors > minPts)
            points[i].core = true;
    }


    // clear image
    image = createImage();
    // temporary container for current cluster
    vector<int> currentCluster;

    // use Prim-like algorithm to find each spanning tree
    // corresponds to a single cluster
    function<void(int, int)> prim;// will be called from below loop, implemented afterward
    prim = [&](int i, int cluster) {
        if (!points[i].core) throw "should not call for boundary or noise.";

        points[i].cluster = cluster; // i is a core
        for (int j = 0; j < size; ++j) {
            if (i == j)
                continue;
            if (paths[i][j]) {// i and j are connected
                points[j].cluster = cluster;
                if (points[j].core) {// both are core, add it to be visited nodes
                    if (find(currentCluster.begin(), currentCluster.end(), j) == currentCluster.end())
                        currentCluster.push_back(j);
                } else {//boundary point
                    points[j].boundary = true;
                }
            }

        }
    };

    //by the end of this loop, a spanning tree is generated for each cluster
    int cluster = -1;
    for (int i = 0; i < points.size(); ++i) {
        if (points[i].core) {
            if (points[i].cluster < 0) {// point[i] hasn't been clustered yet
                cluster++;
                prim(i, cluster);
                for (int j = 0; j < currentCluster.size(); ++j) {

                    int index = currentCluster[j];
                    prim(index, cluster);
                }
                currentCluster.clear();
            }
        }
    }

    cout << "Done clustering. \nThere are " << cluster + 1 << " clusters." << endl;
    plot(points, image,minPts,epsilon, true);

}
