/** Copyright 2021 Wozniamk
    An MPI program to compute factors of a number using 2 MPI processes:

    Rank 0: Operates as manager (performs I/O) 
    Rank 1: Operates as worker  (computes number of factors)

*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <boost/mpi.hpp>

// It is ok to use the following namespace delarations in C++ source
// files only. They must never be used in header files.
using namespace std;
using namespace std::string_literals;

// A convenience namespace for using boost::mpi
using namespace boost;

// A few predefined constants for a readable code
const int NUMBER_TAG  = 1;
const int FACTORS_TAG = 2;

const int MANAGER_RANK = 0;
const int WORKER_RANK  = 1;

// Prototype to keep compiler happy
int getFactorCount(const long long n);

/**
 * This method operates as a worker in concert with the manager process
 * in the following manner:
 *    1. It receives 2 numbers from the manager. If one of them is -1 then
 *       this method returns back.
 *    2. It computes the factors for the 2 numbers by calling the
 *       getFactorCount() helper method that is already implemented in the
 *       supplied starter code.
 *    3. It send the 2 factors back to the manager. 
 */
void doWorkerTasks() {
    mpi::communicator world;
    // long long number;
    std::vector<long long> number(2);  
    // Keep looping until the manager sends -1 
    do {
        /* read number from the manager (rank == 0) */
        world.recv(MANAGER_RANK, NUMBER_TAG, number);
        if (number[0] != -1 && number[1] != -1) {
            number[0] = getFactorCount(number[0]);
            number[1] = getFactorCount(number[1]);

            /* send number of factors back to the manager */
            world.send(MANAGER_RANK, FACTORS_TAG, number);
        }         
    } while (number[0] != -1 && number[0] != -1);
}

/**
 * Starter code method to operate as the manager. The manager operates
 * in the following manner:
 *    1. Repeatedly reads a pair of values from the input data file
 *       1.1. Send 2 numbers to the worker
 *       1.2. Gets 2 factors from the worker and prints it
 *    2. At the end, it sends -1, -1 to the worker to let the worker know
 *       it can stop processing.
 */
void doManagerTasks(const std::string& path) {
    mpi::communicator world;
    std::ifstream data(path);
    if (!data) {
        std::cout << "Unable to open numbers.txt for input. "
                  << "Manager aborting.\n";
    } else {
        std::vector<long long> number(2);     
        // Keep reading pairs of values from the data file
        while ((data >> number[0] >> number[1])) {
            // Send numbers to be converted to the worker
            world.send(WORKER_RANK, NUMBER_TAG, number);
            // Receive factor from the worker
            std::vector<long long> factor(2);
            world.recv(WORKER_RANK, FACTORS_TAG, factor);
            // Print results
            std::cout << "Number " << number[0] << " has " 
                << factor[0] << " factors.\n";
            std::cout << "Number " << number[1] << " has " 
                << factor[1]<< " factors.\n";
        }
    }
    // send -1 to worker to inform worker to end processing
    long long number = -1;
    world.send(WORKER_RANK, NUMBER_TAG, number);
}

//------------------------------------------------------------------
//          DO   NOT  MODIFY  CODE  BELOW  THIS  LINE
//------------------------------------------------------------------

/**
   This is a simple function that can be used to determine the number
   of factors for a given integer i.
*/
int getFactorCount(const long long n) {
    long long i = 0;
    int factorCount = 0;

    for (i = 1; (i <= n); i++) {
        if (!(n % i)) {
            factorCount++;
        }
    }
    return factorCount;
}

int main(int argc, char *argv[]) {
    // Initialize MPI and our world communicator
    mpi::environment env(argc, argv);
    mpi::communicator world;

    // Check to ensure we are running at least 2 processes
    if (world.size() < 2) {
        std::cerr << "This program must be run with at least 2 processes!\n";
    } else if (argc != 2) {
        std::cerr << "Specify name of file to process\n";
    } else {
        // We have atleast 2 processes and a input file to process
        if (world.rank() == MANAGER_RANK) {
            // This is for rank 0
            doManagerTasks(argv[1]);
        } else {
            // This is for non-zero rank
            doWorkerTasks();
        }
    }
    return 0;
}

// End of source code.
