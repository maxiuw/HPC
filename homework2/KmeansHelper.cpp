#ifndef KMEANS_HEPER_CPP
#define KMEANS_HEPER_CPP

/** This file contains the definitions of some of the pre-implemented
 *  helper methods for this program.  This source file essentially
 *  implements the functions declared in kmeans.h
 *
 * Copyright (c) 2021 raodm@miamioh.edu
*/

#include <random>
#include <algorithm>
#include "Kmeans.h"

/**
 * This is a helper method to randomly select a subset of points as
 * the starting point for the centroid.
 *
 * \param[in] data The list of source data points which are being
 * clustered.
 *
 * \param[in] numCentroids The number of centroids to be returned by
 * this method.
 *
 * \return A randomly selected set of points as initial set of
 * centroids.
 */
PointList getInitCentroid(const PointList& data, const int numCentroids) {
    PointList centroids(numCentroids);
    // Pick a random subset of points for use as centroids using the
    // built-in sample algorithm.
    std::sample(std::begin(data), std::end(data), std::begin(centroids),
                numCentroids, std::default_random_engine());
    // Return a randomly selected initial centroids.
    return centroids;
}


/**
 * Just a convenience stream-insertion operator to print a given
 * point.  Each coordinate is separated by a tab.
 *
 * \param[in] os The output stream to where the data is to be written.
 *
 * \param[in] pt The point to be printed
 *
 * \return This method returns the supplied output stream as per the
 * API requirement.
 */
std::ostream& operator<<(std::ostream& os, const Point& pt) {
    for (auto v : pt) {
        std::cout << v << '\t';
    }
    return os;
}

/**
 * A convenience method to compute the Euclidean distance between two
 * points.  See: https://en.wikipedia.org/wiki/Euclidean_distance Note
 * that this method computes the Euclidean distance with any number of
 * dimensions using the capabilities of std::valarray
 *
 * \param[in] p1 The first point to be used to compute Euclidean distance.
 *
 * \param[in] p2 The second point to be used to compute Euclidean distance.
 *
 * \return The Euclidean distance between the two points.
 */
double distance(const Point& p1, const Point& p2) {
    return std::sqrt(std::pow(p1 - p2, 2).sum());        
}

/**
 * Helper method to compute the sum of distances for each point from
 * its assigned centroid.  This is just used to provide some
 * information about the effeciveness of the clustering solution.
 *
 * \param[in] data The input data points (loaded from a TSV) being
 * used for clustering.  These are points to which distances are being
 * calculated to their assigned centroid.
 *
 * \param[in] centroids The centroids to which each data point has
 * been assigned as part of the clustering.
 *
 * \param[in] idx The indexs of the centroid for each data point.
 *
 * \return The total distance between the data points and their
 * centroids.
 */
double getTotDist(const PointList& data, const PointList& centroids,
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


/**
 * This method writes results to a given output stream in the required
 * TSV format.  It also prints the total distance measure at the end.
 *
 * \param[in] data The list of data points (read from a TSV) being
 * used for k-means clustering.
 *
 * \param[in] centroids The current set of centroids to which each
 * data points have been assigned.
 *
 * \param[in] clsIdx The index of the nearest centroid for each data
 * point in the dataList.  This vector must be the one returned by the
 * findClosestCentroid method.
 *
 * \param[out] os The output stream to where the data should be
 * written as a TSV.
 *
 */
void writeResults(const PointList& data, const PointList& centroids,
                  const IntVec& clsIdx, std::ostream& os) {
    os << "#PointType\tCentroidIndex\tCoordinates\n";

    // Print each data point to the output additional columns
    // indicating centroid index and a point type for plotting.
    for (size_t i = 0; (i < data.size()); i++) {
        os << "1\t" << (!clsIdx.empty() ? clsIdx.at(i) : -1)
           << '\t' << data[i] << '\n';
    }

    // Print each centroid to the output with additional columns
    // indicating centroid index and a point type for plotting.
    for (size_t i = 0; (i < centroids.size()); i++) {
        os << "7\t" << i << '\t' << centroids.at(i) << '\n';
    }
    // Print the total distance measure for each point to its assigned
    // centroid.
    const auto totDist = (clsIdx.empty() ? -1 :
                          getTotDist(data, centroids, clsIdx));
    os << "# Total distance measure: " << totDist << std::endl;
}

#endif
