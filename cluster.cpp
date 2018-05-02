#include "cluster.hpp"

static Points points;
static int size;
static double epsilon =8;

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

        vector<int> pCount = {212};//{30, 20, 1000};
        vector<int> minPoints = {2}; //todo: accept also from user

        auto process = [](int minPts, string pointfile, MEASURE m) {
            //points = readPoints(pointfile, false);
//            points = {{7,  0},
//                      {6, 0},
//                      {90, 80},
//                      {90, 5},
//                      {2,  0},
//                      {3,  0},
//                      {0,0},
//                      {3,  2},
//                      {3.5,  3.5},
//                      {90, 75},
//                      {4, 0},
//                      {3, 4}
//            };

            generateRandom(points,1,100,100);//100
            generateRandom(points,1,40,50,30,44);
           // generateRandom(points,70,99,10);
            generateRandom(points, 70,80,5,60,80);
            generateRandom(points, 0,5,20,60,96);
            size = points.size();
            sort(points.begin(), points.end(), lessThan);
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
    } catch (const char *error) {
        cout <<"\nError: "<< error << endl;
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
    line(image, p1, p2, colors.color(2), 2, LINE_AA);
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
void cluster(int minPts, MEASURE m, bool show) {

    Mat image;

    vector<vector<bool> > paths(size);
    for (int i = 0; i < size; i++) {
        // todo: optimize the space
        paths[i] = vector<bool>(size);
    }
    for (int i = 0; i < points.size(); ++i) {
        for (int j = 0; j < points.size(); ++j) {
            paths[i][j] = false;
        }
    }

    // by the end of this loop each core point is marked
    // and if there is a direct path between two points
    // they will be marked true
    cout << "\n";
    for (int i = 0; i < points.size(); ++i) {
        cout << i << ":(" << setfill(' ') << setw(2) << points[i].x << "," << setfill(' ') << setw(2) << points[i].y
             << ")" << "||\t";

        for (int j = i + 1; j < points.size(); ++j) {
            auto dist = getDistance(points[i], points[j], m);
            cout << setfill(' ') << setw(5) << dist << "\t";
            if (dist < epsilon) {
                points[i].neighbors++;
                points[j].neighbors++;
                paths[i][j] = true;
                paths[j][i] = true;
                image = createImage();
                plotPath(image, i, j);

            }
        }
        if (points[i].neighbors > minPts) {
            points[i].core = true;
            if (show)
                plot(points, image, false);

        }
        cout << endl;
    }


    // clear image
    image = createImage();

    vector<int> currentCluster;

    function<void(int, int)> prim;// will be called from below loop, implemented afterward
    prim = [&](int i, int cluster) {
        if (!points[i].core) throw "should not call for boundary or noise.";

        points[i].cluster = cluster; // i is a core
        for (int j = 0; j < size; ++j) {
            if (i == j)
                continue;
            if (paths[i][j]) {// i and j are connected
                points[j].cluster = cluster;
                if (points[j].core) {// both are core, add
                    if (find(currentCluster.begin(), currentCluster.end(), j) == currentCluster.end())
                        currentCluster.push_back(j);
                } else {//boundary
                    points[j].boundary = true;
                }
            }

        }//for j
    };

    //by the end of this loop, a spanning tree is generated for each cluster
    for (int i = 0, c = -1; i < points.size(); ++i) {
        if (points[i].core) {
            if (points[i].cluster < 0) {// point[i] hasn't been clustered yet
                c++;
                prim(i, c);
                for (int j = 0; j < currentCluster.size(); ++j) {

                    int index = currentCluster[j];
                    prim(index, c);
                }

                currentCluster.clear();
            }
        }
    }

    for (int i = 0; i < points.size(); ++i) {
        cout << i << ":(" << setfill(' ') << setw(2) << points[i].x << "," << setfill(' ') << setw(2) << points[i].y
             << ")" << "||\t";
        cout << ((points[i].core) ? "core" : "no  ") << "," << points[i].cluster << endl;
    }

    cout << "Done clustering, there are x clusters." << endl;


    plot(points, image, true);


}
