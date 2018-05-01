#include "cluster.hpp"

static Points points;
static double epsilon = 15;

/****************************
 *
 *
 * main
 *
 *
 ****************************/

int main() {
    try {

        srand(time(nullptr));

        vector<int> pCount = {30};//{30, 20, 1000};
        vector<int> minPoints = {1}; //todo: accept also from user

        auto process = [](int minPts, string pointfile, MEASURE m) {
            points = readPoints(pointfile, false);
//            points = {{1,  1},
//                      {3,  3},
//                      {10, 3},
//                      {80, 80},
//                      {90, 85},
//                      {85, 90},
//                      {50, 50},
//                      {90, 1}};
            cluster(minPts, m);
        };

        for (auto i: pCount) {// for each file with variant number of points
            for (auto minPts: minPoints) { // for variant minPoints

                string pointfile = format("../points_%d.txt", i);

                cout << BCYAN << "\n**************\nk = " << minPts << RESET;

                process(minPts, pointfile, Euclidean);
                //process(minPts, pointfile, Manhattan);
            }
        }
        return 0;
    } catch (string error) {
        cout << error << endl;
        return 1;
    }

}

static Colors colors = Colors();

Mat createImage() {
    Mat image = Mat::zeros(620, 620, CV_8UC3);
    int padding = 20, size = 600, windowSize = size + padding;
    int scale = 4;
    int p2 = padding / 2;

    return image;
}

void plotPath(Mat image, int i, int j) {
    int padding = 10;
    int scale = 6;
    Point p1((points[i].x * scale) + 10, (points[i].y * scale) + 10);
    Point p2((points[j].x * scale) + 10, (points[j].y * scale) + 10);
    line(image, p1, p2, colors.color(3), 2, LINE_AA);
    Mat flipped;
    flip(image, flipped, 0);


}

/****************************
 *
 *
 * DBSCAN algorithm
 *
 *
 ****************************/
void cluster(int minPts, MEASURE m) {
    Mat image = createImage();
    plot(points, image);

    // find the spanning tree to find each separate cluster
    // if all the edge weights of a given graph are the same, then every spanning tree of that graph is minimum.
    bool connected[points.size()][points.size()];
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points.size(); j++) {
            connected[i][j] = false;
        }
    }
    // by the end of this loop each core point is marked
    // and if there is a direct path between two points
    // they will be marked true
    cout << "\n";
    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            auto dist = getDistance(points[i], points[j], m);
            if (dist < epsilon) {
                points[i].neighbors++;
                points[j].neighbors++;
                connected[i][j] = true;
                plotPath(image, i, j);

            }
        }
        if (points[i].neighbors > minPts) {
            points[i].core = true;
            plot(points, image);

        }
        cout << "\n";
    }

    function<void(int, int)> mark;// will be called from below loop, implemented afterward
    mark = [&](int i, int cluster) {
        if (!points[i].core) return;

        points[i].cluster = cluster;
        for (int j = i + 1; j < points.size(); j++) {

            if (connected[i][j] && points[j].core) {// if they are connected and both are core
                cout << "connected " << i << "," << j << endl;
                points[j].cluster = cluster;
                mark(j, cluster);// call recursively

            }
        }//for j
    };

    // by the end of this loop, a spanning tree is generated for each cluster
    for (int i = 0, c = 0; i < points.size(); i++) {
        if (points[i].core) {
            if (!points[i].cluster) {// point[i] hasn't been clustered yet
                c++;
                mark(i, c);
            }
        }
    }

    for (int i = 0; i < points.size(); i++) {
        cout << i << ":(" << setfill(' ') << setw(2) << points[i].x << "," << setfill(' ') << setw(2) << points[i].y
             << ")" << "||\t";
        cout << points[i].core << "," << points[i].cluster << endl;
    }



    plot(points, image);


}
