/** "Copyright 2021 MAciej wozniak" 
 * This source file contains several short (exam-style) methods that
 * involve the use of synchronization and collective communication
 * operations.
 *
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <functional>
#include <boost/mpi.hpp>

// Some convenience namespaces to streamline code
using namespace std;
using namespace boost::mpi;
namespace mpi = boost::mpi;

/**
 * This method is called on all process in an MPI program. Each This
 * method must compute and return the sum and variance of the 'value's
 * (1st parameter) on all processes.
 *
 * \note This method must use only collective communication
 * operations. Hint: all_reduce.
 */
std::tuple<double, double> getMeanAndVar(double value) {
    // Variables whose values are to be calculated.
    double mean = 0, variance = 0;

    // Implement this method to compute the mean and variance of the
    // 'value' on the 'n' processes in the MPI program.  Hint:
    // all_reduce.
    mpi::communicator world;
    mpi::all_reduce(world, value, mean, std::plus<double>());
    mean /= world.size();
    double localvar = (value - mean)*(value - mean);
    mpi::all_reduce(world, localvar, variance, std::plus<double>());   
    variance /= world.size();
    // Return the mean and variance.
    return {mean, variance};
}

/**
 * This method is called on all process in an MPI program. This
 * method must collect all the 'values's from all processes into a
 * vector and return the full list values at all processes.
 *
 * \note This method must use only collective communication
 */
std::vector<int> aggregate(int value) {
    mpi::communicator world;
    std::vector<int> list(world.size());
    // gathering all the processes in process 0 
    mpi::gather(world, value, list, 0);
    // broadcast from 0 - size list 
    mpi::broadcast(world, &list[0], world.size(), 0);
    return list;
}

/**
 * This method is called on every process in an MPI program to print
 * the value associated with the rank.  However, the output appears in
 * random/garbled order.
 *
 * Modify the program to print output in the order of the rank of the
 * process.
 *
 * \param[in] value The value to be printed by the process.
 *
 */
void printInOrder(const std::string& value) {
    mpi::communicator world;
    for (int i = 0; i < world.size(); i++) {
        world.barrier();
        if (world.rank() == i) {
            std::cout << "Value at rank #" << world.rank() << " = "
              << std::quoted(value) << std::endl;
        }
    }
}


// End of source code
