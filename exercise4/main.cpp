/**
 * A simple program to print the 2nd maximum value in a given text file.
 *
 * Copyright (C) 2021 raodm@miamioh.edu
 */

#include <iostream>
#include <string>
#include <fstream>
#include "Person.h"

/**
 * A generic method to read a given type from an input stream and
 * return the second maximum value.  This method uses the default
 * ordering of values as defined by operator<. In addition, this
 * method assumes that the Type supports the stream-insertion
 * (operator<<) and stream-extraction (operator>>) operators
 *
 * \param[in] is The input stream from where values are to be read
 *
 * \return The 2nd maximum value read from the supplied input stream.
 */
template<typename Type>
Type get2ndMax(std::istream& is) {
    // Track the 1st and 2nd largest numbers so that it can be reported back
    // to the client (as required in this project).
    Type max, max2nd;
    is >> max >> max2nd;   // Read first 2 values.
    // Ensure they are in the correct order
    if (max < max2nd) {
        std::swap(max, max2nd);
    }

    // Now process rest of the numbers while tracking the maximum and
    // 2nd maximum values (as required for this project).
    for (Type val; is >> val;) {
        if (max < val) {
            // We found a new maximum value. So update both max and 2nd max
            max2nd = max;  
            max    = val;
        } else if ((max2nd < val) && (val < max)) {
            // We found a number that is less than max but greater
            // than 2nd max. So we update just max2nd.
            max2nd = val;
        }
    }
    // Return the second maximum value.
    return max2nd;
}


/**
 * The main function that reads values from a text file and prints the
 * 2nd maximum value.
 *
 * \param[in] argc The number of command-line arguments.  This program
 * requires only 1 command line argument. 
 *
 * \param[in] argv The actual command-line arguments.
 */
int main(int argc, char *argv[]) {
    // Check to ensure we have the necessary command-line argument.
    if (argc != 2) {
        std::cout << "Specify the filename to use as command-line argument.\n";
        return 1;
    }
    // Open the data file to read values and check to ensure the file
    // is readable.
    std::ifstream is(argv[1]);
    if (!is) {
        std::cout << "Error opening " << argv[1] << std::endl;
        return 1;
    }
    // Get the second maximum value using the helper method.
    auto max2nd = get2ndMax<Person>(is);
    // Print the second max.
    std::cout << "2nd max = " << max2nd << std::endl;    
}

// End of source code

