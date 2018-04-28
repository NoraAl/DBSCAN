#include "cluster.hpp"

static Points points;
static Points centroids;

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

        vector <int> pCount = {20, 1000};
        vector <int> kCount = {2,3,4,7};
        auto process = [](string centroidfile, string pointfile, int k, MEASURE m){
            centroids = readPoints(centroidfile,true);
            points = readPoints(pointfile,false);
            cluster(k,m);
        };

        for (auto i: pCount){// for variant number of points
            for (auto k: kCount){ // for variant k

                string pointfile = format("../points_%d.txt",i);
                string centroidfile = format("../k_%d.txt", k);

                cout <<BCYAN<<"********\nk = "<<k<<"\n********"<<RESET;

                process(centroidfile,pointfile,k,Euclidean);
                process(centroidfile,pointfile,k,Manhattan);

            }
        }
        return 0;
    } catch (string error) {
        cout << error<< endl;
        return 1;
    }

}

/****************************
 *
 *
 * reset each centroid to the
 * average of its cluster
 *
 *
 ****************************/
inline bool updateCentroids() {
    Points oldCentroids = centroids;


    for (int i = 0; i< centroids.size(); i++) {
        int count = 0;
        P currentCentroid = P(-1);

        for (auto p: points) {
            if (p.cluster == i) {
                currentCentroid.x += p.x;
                currentCentroid.y += p.y;
                count++;
            }
        }

        if (count){
            //average
            currentCentroid.x = currentCentroid.x / count;
            currentCentroid.y = currentCentroid.y / count;
            //set current centroid
            centroids[i] = currentCentroid;
        }
        //else{ //if empty cluster, regenerate that centroid, or leave it as is
        //    centroids[i].x = getRandom(minP,maxP);
        //     centroids[i].y = getRandom(minP,maxP);
        //}
    }

    // return true if centroids change; i.e. return false if algorithm converge
    for (int i = 0; i < centroids.size(); i++) {
        if (centroids[i] != oldCentroids[i])
            return true;
    }

    return false;
}

/****************************
 *
 *
 * reassign each point to the
 * nearest cluster
 *
 *
 ****************************/
inline void updateLabels(MEASURE m) {
    double current;
    for (auto &p: points) {
        double min = DBL_MAX;
        int i = 0;
        for (auto c: centroids) {
            current = getDistance(p,c, m);
            if (current < min) {
                min = current;
                p.cluster = i;//initially all clusters are zero
            }
            i++;
        }
    }
}


/****************************
 *
 *
 * k-mean algorithm
 *
 *
 ****************************/
void cluster(int k, MEASURE measure) {

    //centroids.clear();
    for(auto &p: points){
        p.cluster = 0;
    }
    int i = 0;


    do {
        //plot points for each iteration
        plot(points, centroids, measure, k, i );
        updateLabels(measure);
        i++;
    } while (updateCentroids());


    // analysis
    intracluster(measure,k);
    minMax(points,centroids,measure);
}


/****************************
 *
 *
 * intra-cluster distance and the
 * average of all intra-cluster distances.
 *
 *
 ****************************/

void intracluster(MEASURE m, int k) {
    vector<double> distances;

    for (int i = 0; i< centroids.size(); i++) {
        int count = 0; double distance = 0;// reset them each iteration

        for (auto p: points) {
            if (p.cluster == i) {
                count++;
                distance += getDistance(p,centroids[i], m);
            }
        }

        if (count==0){ //empty cluster, regenerate it
            distances.push_back(-1);//
        } else{
            distances.push_back( distance/count);
        }

    }


    cout <<endl<<"Intra-distances ("<<BCYAN<<(m? "Manhattan":"Euclidean")<<RESET<<"):"<<endl;
    double sum = 0;
    for(auto d: distances){
        if (d<0)
            continue;
        sum +=d;
        cout <<BGREEN<<d<<RESET<<"\t";
    }
    sum = sum/k;
    cout <<"\nAverage of intra-distances is: "<<BRED<<sum<<RESET<<endl;

}

