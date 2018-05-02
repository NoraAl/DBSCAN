#include <fstream>
#include "cluster.hpp"


static Colors colors = Colors();
bool operator==(const P &p1, const P &p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(const P &p1, const P &p2) {
    return !(p1 == p2);
}

double getDistance(P p1, P p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

}
void write(Mat image, int k, int i) {
    Mat flipped;
    flip(image, flipped, 0);
    string filename =  "../results/result_";
    filename += to_string(k)+"_"+to_string(i) + ".png";
    imwrite(filename, flipped);
}

void show(Mat image,int size, int minPts, int epsilon, bool last) {
    Mat flipped;
    flip(image, flipped, 0);
    string windowname = "DBSCAN";
    if (last){
        string filename =  "../results/A_";
        filename += to_string(size)+"_"+to_string(minPts) +"_"+to_string(epsilon)+ ".png";
        imwrite(filename,flipped);
        //imshow(windowname, flipped);
       // waitKey(0);
        return;
    }
    imshow(windowname, flipped);
    waitKey(100);

//    while(1){
//        imshow(windowname, flipped);
//        int key = waitKey(1);
//        if (key == ' ')
//            break;
//    }
}



void plot(Points points, Mat image, int minPts, double epsilon, bool last) {
    int scale = 6;
    int padding = 20 / 2;

    //opencv coordinates are not mathematical coordinates, we need to flip the image
    for (auto p:points) {
        int cluster = (p.cluster<0)?0:p.cluster;
        auto p1 = Point((p.x * scale) + padding, (p.y * scale) + padding);

        if (p.core) {//core point
            drawMarker(image, p1,  colors[cluster], MARKER_CROSS , 7, 3);


        }else if(p.boundary){
            drawMarker(image, p1,  colors[cluster],MARKER_TILTED_CROSS , 8, 1);
        }
        else {
            drawMarker(image, p1,  colors.noise(),MARKER_DIAMOND , 2, 2);
        }
    }

    show(image, points.size(),minPts, epsilon, last);
}

void plotPath(Points points, Mat image, int i, int j) {
    int padding = 10;
    int scale = 6;
    Point p1((points[i].x * scale) + 10, (points[i].y * scale) + 10);
    Point p2((points[j].x * scale) + 10, (points[j].y * scale) + 10);
    line(image, p1, p2, colors.color(2), 2, LINE_AA);
    Mat flipped;
    flip(image, flipped, 0);


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
    point.neighbors =(centroids? -1:0);

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