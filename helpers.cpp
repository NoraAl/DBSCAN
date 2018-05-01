#include <fstream>
#include "cluster.hpp"

// overload operators
bool operator==(const P &p1, const P &p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(const P &p1, const P &p2) {
    return !(p1 == p2);
}

double getDistance(P p1, P p2, MEASURE m) {
    if (m == Euclidean)
        return getEuclidean(p1, p2);
    return getManhattan(p1, p2);

}

inline double getEuclidean(P p1, P p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

}

inline double getManhattan(P p1, P p2) {
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

inline void getMinMaxPerCluster(Points p, int i, int j){

}

void minMax(Points &points, Points &centroids, MEASURE m){
    int k = centroids.size();
    double minMatrix[k][k];
    double maxMatrix[k][k];
    //     0 1 ...... k-1
    //   0| | | .....|   |
    //   1| | | .....|   |
    //   .| | | .....|   |
    //   .| | | .....|   |
    // k-2| | | .....|   |
    // k-1| | | .....|   |
    for (int i = 0; i <k; i++)
        for (int j = 0; j<k; j++){
            minMatrix[i][j] = DBL_MAX;
            maxMatrix[i][j] = DBL_MIN;
        }

    for (int i = 0; i <k; i++)// current neighbors
        for (int j = 0; j<k; j++){//every other neighbors

            if (i>=j)
                continue;

            else {
                for (auto p: points){
                    if (p.neighbors == i){
                        for (auto otherP: points){
                            if (otherP.neighbors == j){
                                double current = getDistance(p,otherP,m);
                                if ( current< minMatrix[i][j])
                                    minMatrix[i][j] = current;
                                if (current > maxMatrix[i][j])
                                    maxMatrix[i][j] = current;
                            }
                        }
                    }
                }
            }

        }

    cout <<BGREEN<< "Minimum distances: "<< endl;
    for (int i = 0; i <k; i++) {
        for (int j = 0; j < k; j++) {
            cout << fnum(minMatrix[i][j]);

        }
        cout <<endl;
    }
    cout <<RESET<<endl;

    cout <<BRED<< "Max distances: "<< endl;
    for (int i = 0; i <k; i++) {
        for (int j = 0; j < k; j++) {
            cout << fnum(maxMatrix[i][j]);

        }
        cout <<endl;
    }
    cout <<RESET<<endl;

}


string fnum(double num){
    stringstream stream;
    if (num == DBL_MIN || num == DBL_MAX){
        stream << setfill(' ') << setw(10) << "*";
    } else {
        stream << setfill(' ') << setw(10) << num;
    }
    return stream.str();
}

void write(Mat image, MEASURE m,int k, int i) {
    Mat flipped;
    flip(image, flipped, 0);
    string filename = m ? "../results/Manhattan_" : "../results/Euclidean_";
    filename += to_string(k)+"_"+to_string(i) + ".png";
    imwrite(filename, flipped);
}

void show(Mat image) {
    Mat flipped;
    flip(image, flipped, 0);
    string windowname = "DBSCAN";


    while(1){
        imshow(windowname, flipped);
        int key = waitKey(1);
        if (key == ' ')
            break;
    }
}



void plot(Points points, Mat image) {
    int scale = 6;
    int padding = 20 / 2;
    Colors colors = Colors();

    //opencv coordinates are not mathematical coordinates, we need to flip the image
    for (auto p:points) {
        if (p.core) {//core point
            circle(image, Point((p.x * scale) + padding, (p.y * scale) + padding), 7, colors.shadow(), -1,LINE_AA);
            circle(image, Point((p.x * scale) + padding, (p.y * scale) + padding), 5, colors[p.cluster], -1, LINE_AA);

        }
        else {//boundary and noise
            //todo: make boundary
            circle(image, Point((p.x * scale) + padding, (p.y * scale) + padding), 4, colors.noise(), -1,LINE_AA);
        }
    }

    show(image);
}


void printPoints(Points points) {
    int i = 0;
    for (auto p: points) {
        i++;
        cout  << "" << p.x << "," << p.y << "\n" ;
    }
    cout << endl;
}

Points readPoints(string filename, bool centroids, char separator) {
    Points points;
    P point(0,0);
    point.neighbors =(centroids? -1:0);;
    ifstream file(filename, ifstream::in);
    if (!file) {
        throw "Not a valid file name.";
    }

    if (file.is_open())
    {
        string line, x, y ;
        while (getline(file, line)) {
            stringstream lineStream(line);
            getline(lineStream, x, separator);
            getline(lineStream, y);

            if(!x.empty() && !y.empty()) {
                point.x = atof(x.c_str());
                point.y = atof(y.c_str());

                points.push_back(point);
            }
        }
        file.close();
    } else {
        throw "couldn't open " + filename + "\n";
    }

    return points;

}