/**
 * A simple top-level tester for testing the different matrix
 * operations.
 *
 * Copyright (C) 2021 raodm@miamioh.edu
 */

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cmath>
#include "Matrix.h"

// Convenience namespace for C++ string literals
using namespace std::string_literals;

/**
 * A simple sigmoid function.
 *
 * \param[in] val The value whose sigmoid value is to be returned.
 *
 * \return The sigmoid value for the given val.
 */
double sigmoid(const double val) {
    return 1. / (1. + std::exp(-val));
}

/**
 * A simple inverse-sigmoid function.
 *
 * \param[in] val The value whose inverse sigmoid value is to be
 * returned.
 *
 * \return The inverse sigmoid value for the given val.
 */
double invSigmoid(const double val) {
    return sigmoid(val) * (1 - sigmoid(val));
}

/**
 * Helper method to load matrix data from a given file. This method
 * essentially tests the operator>> (stream extraction operator) for
 * the matrix class.
 *
 * \param[in] file The file from where the data is to be loaded.  If
 * the file cannot be opened, then this method throws an exception.
 */
Matrix load(const std::string& path) {
    // Open the specified file.
    std::ifstream matFile(path);
    // Check to ensure that the file is ok
    if (!matFile) {
        throw std::runtime_error("Error opening file " + path);
    }
    // Create a temporary matrix and have it read the data for itself
    // from the file.
    Matrix mat;
    matFile >> mat;
    // Return the newly created/loaded matrix
    return mat;
}

int main(int argc, char *argv[]) {
    // if (argc < 3) {
    //     std::cout << "Specify command-line arguments for testing.\n";
    //     return 1;
    // }

    // The second command-line argument is always a reference
    // matrix. So load it first.
    const auto mat1 = load(argv[2]);
    std::cout << mat1;
    // Based on the first command-line argument, call different operators.
    // switch (argv[1][0]) {
    // case '<': std::cout << mat1; break;
    // case '+': std::cout << mat1 + load(argv[3]);    break;
    // case '-': std::cout << mat1 - load(argv[3]);    break;
    // case '*': std::cout << mat1 * load(argv[3]);    break;
    // case 'd': std::cout << mat1.dot(load(argv[3])); break;
    // case 't': std::cout << mat1.transpose();        break;
    // case 's': std::cout << mat1.apply(sigmoid);     break;
    // case 'i': std::cout << mat1.apply(invSigmoid);  break;
    // default:  std::cout << "Invalid matrix operation.\n";
    // }
    return 0;
}



