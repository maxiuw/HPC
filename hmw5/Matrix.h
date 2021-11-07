#ifndef MATRIX_H
#define MATRIX_H

/** \file matrix.h A 2-D matrix class.  This file contains the
    declaration for a simple 2-D matrix class along with several
    convenience operators to streamline its use.

    Copyright (C) 2015 raodm@miamiOH.edu
*/

#include <iostream>
#include <functional>
#include <vector>
#include <cassert>

/** Shortcut for the value of each element in the matrix */
using Val = double;

/** Short cut to a 2-d vector double values to streamline the code */
using TwoDVec = std::vector<std::vector<Val>>;

/** A matrix class to perform basic matrix operations.

    The class essentially encapsulates a 2-d matrix of double values
    and performs the following matrix operations:

    <ul>
    <li>Create a matrix of given dimensions.</li>

    <li>Matrix multiplication using Block matrix multiplication.</li>

    <li> Stream insertion and extraction operators to conveniently
    load and print values.</li>
    
    </ul>
*/
class Matrix : public TwoDVec {
    /** Stream insertion operator to ease printing matrices
     *
     * This method prints the dimension of the matrix and then prints
     * the values in a row-by-row manner.  The output is setup to be
     * consistent with the stream extraction operator.
     *
     * \param[out] os The output stream to where the data is to be
     * written. This could be any output stream -- for example,
     * a std::ofstream, std::ostringstream, or std::cout.
     *
     * \param[in] matrix The matrix to be written.
     *
     * \return As per convention, this method returns the supplied
     * output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

    /** Stream extraction operator to ease reading matrices
     *
     * This method reads the dimension of the matrix and then reads
     * the values in a row-by-row manner.  The input is assumed to be
     * that one generated by the stream insertion operator.
     *
     * \param[in/out] in The input stream to where the data is to be
     * read. This could be any input stream -- for example,
     * a std::ifstream, std::istringstream, or std::cin.
     *
     * \param[in] matrix The matrix to be read.
     *
     * \return As per convention, this method returns the supplied
     * input stream.
     */
    friend std::istream& operator>>(std::istream& is, Matrix& matrix);

public:
    /**
     * Constructor to create and initialize a matrix.
     *
     * \param[in] rows The number of rows to be created in the
     * matrix.
     *
     * \param[in] cols The number of cols to be created in the matrix.
     *
     * \param[in] initVal The inital value to be set for each entry in
     * the matrix.
     */
    explicit Matrix(const size_t rows = 0, const size_t cols = 0,
                    const Val initVal = 0);

    /**
     * Returns the height or number of rows in this matrix.
     *
     * \return Returns the height or number of rows in this matrix.
     */
    int height() const { return size(); }

    /**
     * Returns the width or number of columns in this matrix.
     *
     * \return Returns the width or number of columns in this matrix.
     */
    int width() const { return (height() > 0) ? front().size() : 0; }
    
    /**
     * Creates a new matrix in which each value is obtained by
     * applying a given unary operator to each entry in the matrix.
     *
     * \param[in] operation The unary operation to be used to create
     * the given matrix.
     */
    template<typename UnaryOp>
    Matrix apply(const UnaryOp& operation) const {
        // If the matrix is empty, then we have nothing to do.
        if (empty()) {
            return *this;  // return copy of empty matrix.
        }
        // Now apply the specified operation to each element and store it
        // in the new matrix
        Matrix result = *this;  // Initialize to current values.
        // Apply unary operation to each element in the result
        for (auto& row : result) {
            for (auto& val : row) {
                val = operation(val);
            }
        }
        // The resulting matrix after applying specified operations.
        return result;
    }

    /**
     * Creates a new matrix in which each value is obtained by
     * applying a given binary operator to each entry in this matrix
     * and another matrix.
     *
     * \param[in] other The other matrix to be used. Note that the
     * other matrix must be exactly the same dimension of this this.
     * Otherwise, this method throws an exception.
     *
     * \param[in] operation The binary operation to be used to create
     * each value in the given matrix.
     */    
    template<typename BinaryOp>
    Matrix apply(const Matrix& other, const BinaryOp& operation) const {
        // Check to ensure the number of rows are the same.
        assert(size() == other.size());
        // If the matrix is empty, then we have nothing to do.
        if (empty()) {
            return *this;  // return copy of empty matrix.
        }
        // Ensure the number of columns match.
        assert(front().size() == other.front().size());
        // Now apply the specified operation to each element and store it
        // in the new matrix
        Matrix result = *this;  // Initialize to current values.
        // Here we use index so that we can access the corresponding
        // element in the other matrix as well.
        for (size_t row = 0; (row < size()); ++row) {
            for (size_t col = 0; (col < (*this)[row].size()); ++col) {
                // Recollect result is initialized to values of this
                // matrix. So we use result to reduce the number of
                // different values accessed.
                result[row][col] = operation(result[row][col], other[row][col]);
            }
        }
        // The resulting matrix after applying specified operations.
        return result;
    }

    /**
     * Operator to add two matrices with the same dimensions together.
     *
     * \param[in] rhs The other matrix to be used.  This matrix must
     * have the same dimension as this matrix.  Otherwise this method
     * throws an excpetion.
     *
     * \return The resulting matrix in which each value has been
     * computed by adding the corresponding values from \c this and
     * rhs.
     */
    // Matrix operator+(const Matrix& rhs) const {
    //     return apply(rhs, [](const auto& v1, const auto& v2) {
    //                           return v1 + v2; });
    // }
    Matrix operator+(const Matrix& rhs) {
        // int const rows = rhs.height();
        // int const cols = rhs.width();
        // Matrix M(rows, cols);
        for (int row = 0; row < this->height(); row++) {
            for (int col = 0; col < this->width(); col++) {
                // M[row][col] = this[row][col] + rhs[row][col];
                (*this)[row][col] += rhs[row][col];                
            }
        }
        return *this;   
    }

    /**
     * Operator for computing the Hadamard product of two matrices
     * with the same dimensions.
     *
     * \param[in] rhs The other matrix to be used.  This matrix must
     * have the same dimension as this matrix.  Otherwise this method
     * throws an excpetion.
     *
     * \return The resulting matrix in which each value has been
     * computed by multiplying the corresponding values from \c this
     * and rhs.
     */
    // Matrix operator*(const Matrix& rhs) const {
    //     return apply(rhs, [](const auto& v1, const auto& v2) {
    //                           return v1 * v2; });
    // }
    Matrix operator*(const Matrix& rhs) {
        // int const rows = rhs.height();
        // int const cols = rhs.width();
        // Matrix M(rows, cols);
        for (int row = 0; row < this->height(); row++) {
            for (int col = 0; col < this->width(); col++) {
                // M[row][col] = this[row][col] + rhs[row][col];
                (*this)[row][col] *= rhs[row][col];                
            }
        }
        return *this;   
    }

    /**
     * Operator for computing the Hadamard product of two matrices
     * with the same dimensions.
     *
     * \param[in] rhs The other matrix to be used.  This matrix must
     * have the same dimension as this matrix.  Otherwise this method
     * throws an excpetion.
     *
     * \return The resulting matrix in which each value has been
     * computed by multiplying the corresponding values from \c this
     * and rhs.
     */
    // Matrix operator*(const Val val) const {
    //     return apply([val](const auto& v) { return v * val; });
    // }
    Matrix operator*(const Val val) {
        // Matrix M(rows, cols);
        for (int row = 0; row < this->height(); row++) {
            for (int col = 0; col < this->width(); col++) {
                // M[row][col] = this[row][col] + rhs[row][col];
                (*this)[row][col] *= val;               
            }
        }
        return *this;   
    }
    
    /**
     * Operator to subtract two matrices with the same dimensions.
     *
     * \param[in] rhs The other matrix to be used.  This matrix must
     * have the same dimension as this matrix.  Otherwise this method
     * throws an excpetion.
     *
     * \return The resulting matrix in which each value has been
     * computed by subtracting the corresponding values from \c this
     * and rhs.
     */
    // Matrix operator-(const Matrix& rhs) const {
    //     return apply(rhs, [](const auto& v1, const auto& v2)  {
    //                           return v1 - v2; });
    // }
    Matrix operator-(const Matrix& rhs) {
        assert(this->width() == rhs.width() && this->height() == rhs.height());
        // Matrix M(rows, cols);
        for (int row = 0; row < this->height(); row++) {
            for (int col = 0; col < this->width(); col++) {
                // M[row][col] = this[row][col] + rhs[row][col];
                (*this)[row][col] -= rhs[row][col];                
            }
        }
        return *this;   
    }


    /**
     * Performs the dot product of two matrices. This method has a
     * O(n^3) time complexity.
     *
     * \param[in] rhs The other matrix to be used.  This matrix must
     * have the same number of rows as the number of columns in this
     * matrix.  Otherwise this method throws an excpetion.
     *
     * \return The resulting matrix in which each value has been
     * computed by multiplying the corresponding values from \c this
     * and rhs.
     */
    Matrix dot(const Matrix& rhs) const;

    /**
     * Returns the transpose of this matrix.
     */
    Matrix transpose() const;
};


#endif
