#ifndef KMEANS_CPP
#define KEMANS_CPP

/**
 * A standard/simple k-means clustering program. This program reads
 * data from a Tab-Separated-Value (TSV) file and prints clustering
 * resuts to standard output.
 *
 * Copyright (C) 2021 raodm@miamioh.edu
 *
 * Note: The inputs to this program are supplied
 * as command-line arguments:
 *    1. The first argument is assumed to be the input
 *       Tab-Separated-Value (TSV) file from where the first 'n'
 *       columns are to be used as the data to be clustered.
 *    2. The second argument is the number of columns (starting with
 *       the first column) in the TSV file to be used as the data to
 *       be clustered. This value must be at least 1.
 *    3. The third argument is the number of centroids to use for
 *       clustering.  If this value is zero, just print the data
 *       read for the specified number of columns by calling writeResults()
 *       method (already implemented).
 *
 * Optionally the output can be visualized using Gnuplot via the
 * following command:
 *   $ ./main old_faithful.tsv 2 2 > temp.tsv
 *   $ gnuplot -e 'plot "temp.tsv" using 3:4:1:2 with points pt var lc var; pause -1'
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <valarray>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <tuple>
#include <cassert>
#include "Kmeans.h"

/**
 * Finds the closest centroid (from a given list of centroids) for
 * each data item to be clustered.  For example, given
 * 
 * `dataList  = {{1, 2}, {3, 4}, {5, 6}, {7, 8}}` and
 * `centroids = {{2, 2}, {6, 6}}`
 *
 * this method would return `{0, 0, 1, 1}` which are the index of the
 * closest point in `centroids` for each point in `dataList`
 *
 * \param[in] dataList The list of data points being used for k-means
 * clustering.
 *
 * \param[in] centroids The current set of centroids. This method
 * finds the closest centroid from this list.
 *
 * \return A list-of-indexs corresponding to the centroids for each
 * entry in the supplied dataList.
 */
IntVec findClosestCentroid(const PointList& dataList,
                           const PointList& centroids) {
    // Create the vector (of appropriate size) to be returned by this
    // method.
    IntVec idx(dataList.size());
    int i = 0;  // Variable to track index in the above vector.

    // Compute index of the closest centroid for each data point.
    for (const auto& data : dataList) {
        // Track the index and distance of the nearest
        // centroid. min.first is index and min.second is
        // corresponding distance.
        std::pair<int, double> min = {0, distance(data, centroids.at(0))};
        for (size_t i = 1; (i < centroids.size()); i++) { // 1
            if (auto dist = distance(data, centroids[i]); dist < min.second) {
                // We would another centroid that is closer. So track it.
                min = {i, dist};
            }
        }
        // Now minCent has the index of the nearest centroid. Store it.
        idx[i++] = min.first;
        
    }
    // Return the index of nearest centroid for each data point.
    return idx;
}

/**
 * This method recomputes the new set of centroids based on the
 * current set of centroids assigned to each point.  A centroid is
 * computed by averaging the coordinates of each point associated with
 * it.
 *
 * \see findClosestCentroid
 *
 * \param[in] idx The index of the nearest centroid for each data
 * point in the dataList.  This vector must be the one returned by the
 * findClosestCentroid method.
 *
 * \param[in] dataList The list of data points being used for k-means
 * clustering.
 *
 * \param[in] numCentroids The number of centroids to be computed by
 * this method.
 *
 * \return The coordinates for 'numCentroids' centroids.
 */
PointList computeNewCentroid(const IntVec& idx, const PointList& dataList,
                             int numCentroids) {
    // Values to be computed below.
    int pointsize = dataList[0].size();  // size of the point 
    std::valarray<double> pointholder(pointsize);  // val array of 0
    PointList centroids(numCentroids, pointholder); 
    // incorrect version of centroids PointList centroids(numCentroids, dataList[0])

    // Counts are tracked to compute averages in the next loop.
    std::vector<int> counts(numCentroids);
    // Add up the points based on the current centroids they are
    // assigned to.
    // std::cout << centroids[1];
    
    for (size_t i = 0; (i < idx.size()); i++) {
        // const int cent   = idx[i];
        centroids[idx[i]] += dataList[i];
        counts[idx[i]]++;
    }
    // Compute centroids by averaging along each axis. The counts from
    // the previous loop are used for computing average.

    for (size_t i = 0; (i < centroids.size()); i++) {
        // std::cout << centroids[i] << " counts " << counts[i] << "centroid " << i << '\n';

        centroids[i] /= counts[i];
    }
    Point p = dataList[0];
    
    for (size_t i = 0; i <dataList.size();i++) {
        if (i == 0) {
            continue;
        }
        p += dataList[i];
    }

    // std::cout << p << "sum of all the points, should be equal to sum of n centroids\n";
    return centroids;
}

/**
 * The core clustering method that iteratively refines the
 * clustering. Specifically this method:
 *
 *   1. Starts with a set of centroids initialized to points randomly
 *      selected from the input data.
 *
 *   2. Repeatedly performs the following operation until centroids
 *      don't change (or maxReps are completed):
 *         2.1. Find nearest centroid to each point in `dataList`
 *              thereby computing an interation of clustering.
 *         2.2. Recomputes centroids based on the above clustering.
 *
 * \return This method returns the centroids and the indexs assigned
 * to each centroid as a single tuple.
 */
std::tuple<PointList, IntVec>
cluster(const PointList& dataList, int numCentroids, const int maxReps = 100) {
    // Do the clustering only if the number of centroids is > 0
    if (numCentroids == 0) {
        return {};
    }
    // Get initial set of randomly selected centroids.  These are
    // updated/refined in the loop below.
    auto centroids = getInitCentroid(dataList, numCentroids);
    // The index of centroids/clusters assigned to each data point to
    // be returned by this method.
    std::vector<int> idx(dataList.size());

    // A convenience lambda used in the loop below to check if 2 lists
    // of points are the same.  This lamba compares a pair of points
    // to determine if they are equal or not.
    auto isSame = [](const Point& p1, const Point& p2)
                  { return (p1 == p2).min(); };

    // Run the clustering iterations until they converge.
    for (auto rep = 0; (rep < maxReps); rep++) {
        // std::cout << "Rep: " << rep << std::endl;        
        // Find the index of the closest centroids for each data point.
        idx  = findClosestCentroid(dataList, centroids);
        // Recompute centroids based on updated indexs
        auto newcenters = computeNewCentroid(idx, dataList, numCentroids);
        if (std::equal(std::cbegin(centroids), std::cend(centroids),
                       std::cbegin(newcenters), isSame)) {
            break;
        }
        // Centroids changed. Onto the next iteration.
        centroids = newcenters;
    }
    // Return the result of k-means clustering back to the caller
    return {centroids, idx};
}

PointList loadData(const std::string& tsvPath, int numCols) {
    std::ifstream tsv(tsvPath);
    PointList data;
    for (std::string line; std::getline(tsv, line);) {
        if (line.empty() || line.front() == '#') {
            continue;   // Ignore empty or comment lines.
        }
        // Extract the first 'n' columns into a Point
        Point p(numCols);
        // istream eases extracting strings and numbers in loop below
        std::istringstream is(line);
        // Reach each coordinate for the data point.
        for (auto& v : p) {
            is >> v;
        }

        // Add point to the list of data points to be returned
        data.push_back(p);
    }

    // Return the 'n' columns of data loaded from the tsv
    return data;
}


/**
 * The main function that coordinates the various operations of
 * clustering.
 *
 * \param[in] argc The number of command-line arguments.  This program
 * requires exactly 3 command-line arguments.
 *
 * \param[in] argv The actual list of command-line arguments that are
 * assumed to be in the following order:
 *
 *    1. The first argument is assumed to be the input
 *       Tab-Separated-Value (TSV) file from where the first 'n'
 *       columns are to be used as the data to be clustered.
 *
 *    2. The second argument is the number of columns (starting with
 *       the first column) in the TSV file to be used as the data to
 *       be clustered.  This value must be at least 1.
 *
 *    3. The third argument is the number of centroids to use for
 *       clustering. Can be zero or more.
 */
int main(int argc, char *argv[]) {
    // Check to ensure we have the necessary arguments specified.
    if (argc < 4) {
        std::cout << "Usage: <InputTSV> <NumCols> <NumCentroids>\n";
        return 1;  // Insufficient arguments.
    }

    // Load the specified number of columns from input TSV
    PointList vals = loadData(argv[1], std::stoi(argv[2]));
    // Do the clustering, only if number of centroids is > 0
    auto [centroids, idx] = cluster(vals, std::stoi(argv[3]));
    // Print the clustering results
    writeResults(vals, centroids, idx);
    return 0;
}

// End of source code

#endif

