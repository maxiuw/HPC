#ifndef MATRIX_CPP
#define MATRIX_CPP

#include <cassert>
#include <vector>
#include "Matrix.h"

Matrix::Matrix(const size_t row, const size_t col, const Val initVal) :
    std::vector<std::vector<Val>>(row, std::vector<Val>(col, initVal)) {
}

// Operator to write the matrix to a given output stream
std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    // Print the number of rows and columns to ease reading
    os << matrix.height() << " " << matrix.width() << '\n';
    // Print each entry to the output stream.
    for (auto& row : matrix) {
        for (auto& val : row) {
            os << val << " ";
        }
        // Print a new line at the end of each row just to format the
        // output a bit nicely.
        os << '\n';
    }
    return os;
}

std::istream& operator>>(std::istream& is, Matrix& matrix){
    std::string i,j; // rows and cols 
    is>>i>>j;
    int rows = std::stoi(i);
    int cols = std::stoi(j);
    Matrix tmp(rows,cols);
    for (size_t r = 0; r < rows; r++){
        for (size_t c = 0; c <cols; c++){
            std::string placeholder;
            is>>placeholder;
            std::cout << placeholder;
            tmp[r][c] = std::stod(placeholder); 
        }
    }
    matrix = tmp;
    return is;
}


#endif
