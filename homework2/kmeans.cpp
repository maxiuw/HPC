// Copyright (c) 2021 Maciej Wozniak

#include "Kmeans.h"
#include <time.h>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


/**
 * reading the tsv file and returning poits
 *
 * \param[in] temp tsv file name 
 *
 * \param[in] n_cols number of columns we want to read (i.e. dims
 * of the point)
 *
 * \return list of the points 
 */
const PointList splitline(std::string &temp, int n_cols) {
    // define point vector
    PointList points;
    std::ifstream is(temp);
    // get the first row (columns) so
    std::string l;
    getline(is, l, '\n');
    // vector of points
    // getting number from the row and pushing it to the point
    while (getline(is, l, '\n')) {
        Point n(n_cols);
        std::string numb;
        int i = 0;
        for (char ch : l) {
            // std::cout << ch;
            if (ch == '\t') {
                n[i] = std::stod(numb);
                i++;
                numb = "";
            } else {
                numb.push_back(ch);
            }
            if (i == n_cols) 
                break;                     
        }
        if (i != n_cols) 
            n[i] = std::stod(numb);                        
        points.push_back(n);
    }
    return points;
}


/**
 * gets centroids idx for each point
 *
 * \param[in] points points  
 *
 * \param[in] cents list of centroid coordinates
 * 
 * \return list of indexes of centroids of each point  
 */
IntVec getPointsCentroids(const PointList &points, PointList &cents) {
    IntVec points_centroids(points.size());
    // for all points check all centroids and choose the one with smallest dist
    for (size_t p = 0; p < points.size(); p++) {
        double dist = std::numeric_limits<int>::max();
        int ix = 0;
        for (size_t i = 0; i < cents.size(); i++) {
            double new_dist = distance(points[p], cents[i]);
            // if dist to new centroid is smaller, this is the cluster 
            if (new_dist < dist) {
                ix = i;
                dist = new_dist;
            }
        }
        points_centroids[p] = ix;
    }
    return points_centroids;
}

/**
 * calculates new centroids
 *
 * \param[in] points points  
 *
 * \param[in] pointscentroids list of centroid indxs for each point
 * 
 * \param[in] numb_centroids number of centroids
 * \param[in] n_cols dimensions of the points
 * \return new coordinates of the centroids 
 */

PointList NewCentorids(const PointList &points,
     std::vector<int> &pointscentroids,
     int &numb_centroids, int &n_cols) {
    // if i use just final points>error that vct                               
    PointList finalPoints(numb_centroids);                                  
    std::valarray<Point> newCentroids(numb_centroids);
    std::valarray<double> nPointsForCentroid(numb_centroids);
    // looping through the points
    for (size_t i = 0; i < pointscentroids.size(); i++) {
        // adding points to the centroids 
        if (newCentroids[pointscentroids[i]].size() == 0) {
            newCentroids[pointscentroids[i]] = points[i];
            nPointsForCentroid[pointscentroids[i]] = 1.0;
        } else {
            newCentroids[pointscentroids[i]] += points[i];
            nPointsForCentroid[pointscentroids[i]] += 1.0;
        }
    }
    // divinding sum of the points by n of points beloging to that centr
    for (size_t i = 0; i < nPointsForCentroid.size(); i++) {
        double k = nPointsForCentroid[i];
        std::valarray<double> divider(k, n_cols);
        finalPoints[i] = newCentroids[i] / divider;
        // std::cout << newCentroids[i] << " divid  " << k <<  '\n';
    }
    return finalPoints;
}

/**
 * generates random centroids
 *
 * \param[in] points points  
 *
 * \param[in] pointscentroids list of centroid indxs for each point
 * 
 * \param[in] numb_centroids number of centroids
 * \param[in] n_cols dimensions of the points
 * \return new coordinates of the centroids 
 */
double getTotalDist(const PointList& data, const PointList& centroids,
                  const IntVec& idx) {
    if (data.size() != idx.size()) {
        std::cout << "The getTotDist() method expects an centroid-index set "
                  << "in idx vector, for each data point\n";
        // Return an invalid value as the data supplied is incorrect.
        return -1;
    }
    // Compute total distance.
    double dist = 0;
    for (size_t i = 0; (i < data.size()); i++) {
        dist += distance(data[i], centroids.at(idx[i]));
    }
    // Return the total distance
    return dist;
}

void printResults(const PointList& data, const PointList& centroids,
                  const IntVec& clsIdx) {
    // std::ofstream os("temp.tsv");
    // os << "#PointType\tCentroidIndex\tCoordinates\n";
    std::cout << "#PointType\tCentroidIndex\tCoordinates\n";

    // Print each data point to the output additional columns
    // indicating centroid index and a point type for plotting.
    for (size_t i = 0; (i < data.size()); i++) {
        std::cout << "1\t" << (!clsIdx.empty() ? clsIdx.at(i) : -1)
           << '\t' << data[i] << '\n';
        // os << "1\t" << clsIdx.at(i) << '\t' << data[i] << '\n';
    }

    // Print each centroid to the output with additional columns
    // indicating centroid index and a point type for plotting.
    for (size_t i = 0; (i < centroids.size()); i++) {
        std::cout << "7\t" << i << '\t' << centroids.at(i) << '\n';
        // os << "7\t" << i << '\t' << centroids.at(i) << '\n';
    }
    // Print the total distance measure for each point to its assigned
    // centroid.
    const auto totDist = (clsIdx.empty() ? -1 :
                          getTotalDist(data, centroids, clsIdx));
    std::cout << "# Total distance measure: " << totDist << std::endl;
}

std::pair<PointList, IntVec> MainLoop(const PointList &points, 
        int &numb_centroids, int &n_cols) {
    int maxiterations = 100;
    PointList cents = getInitCentroid(points, numb_centroids);
    // centroids for each point
    std::vector<int> pointscentroids;  
    // centroids optimization  
    int currentIteration = 0;
    while (currentIteration < maxiterations) {
        pointscentroids = getPointsCentroids(points, cents);
        PointList newCent =
            NewCentorids(points, pointscentroids, numb_centroids, n_cols);
        double d = 0;  // distance 
        for (int c = 0; c < numb_centroids; c++) {
            d += distance(cents[c], newCent[c]);
        }
        if (d == 0) {
            cents = newCent;
            break;
        }
        // std::cout << currentIteration << '\n';
        cents = newCent;
        currentIteration++;
    }
    return std::make_pair(cents, pointscentroids);
}

int main(int argc, char *argv[]) {    
    // Check to ensure we have a data file specified.
    if (argc < 2) {
        std::cout << "Specify data file as command-line argument.\n";
        return 1;
    }
    // Open the data file and ensure it is readable.
    std::ifstream file(argv[1]);

    if (!file.good()) {  // check if file was open successfully
        std::cerr << "unable to read the file\n";
        return 1;
    }
    std::string filename = argv[1];
    std::string n_centroids = argv[3];
    int n_cols = std::stoi(argv[2]);
    // returning vector of n - D points
    const PointList points = splitline(filename, n_cols);
    // initial centroidss
    int numb_centroids = std::stoi(n_centroids);
    if (numb_centroids == 0) {
        IntVec v;
        PointList c;
        printResults(points, c, v);
        return 0;
    }
    std::pair<PointList, IntVec> pair = MainLoop(points,
     numb_centroids, n_cols);
    printResults(points, pair.first, pair.second);
    return 0;
}
