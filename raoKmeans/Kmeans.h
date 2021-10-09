#ifndef KMEANS_H
#define KMEANS_H

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
 *   $ gnuplot -e 'set terminal png; set output "temp.png"; plot "temp.tsv" using 3:4:1:2 with points pt var lc var;'
 */

#include <valarray>
#include <vector>
#include <iostream>

// A few synonyms for data types to streamline the API and helper
// code.

/** Here we use a std::valarray to represent a point.  Note that a
 *  point can be 2-D, 3-D, etc.  We use a std::valarray as some of the
 *  math operations become easier.
*/
using Point = std::valarray<double>;

/** A list of data points to be clustered by this program. This list
 * is also used to represent the list of centroids obtained from
 * clustering.  The data in this list is loaded is used as the source
 * data types and for the centroids.
*/
using PointList = std::vector<Point>;

/** This is an optional list is used to hold the index-of-centroid
 *  associated with each data item being clustered.
*/
using IntVec = std::vector<int>;

/**
 * This is a helper method to randomly select a subset of points as
 * the starting point for the centroid.
 *
 * \param[in] data The list of source data points (loaded from a TSV
 * file) which are being clustered.
 *
 * \param[in] numCentroids The number of centroids to be returned by
 * this method.
 *
 * \return A randomly selected set of points as initial set of
 * centroids.
 */
PointList getInitCentroid(const PointList& data, const int numCentroids);

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
std::ostream& operator<<(std::ostream& os, const Point& pt);

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
double distance(const Point& p1, const Point& p2);

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
                  const IntVec& clsIdx, std::ostream& os = std::cout);

#endif
