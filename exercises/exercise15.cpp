// Copyright (C) 2014 raodm@miamiOH.edu

#include <omp.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <unordered_map>

// Prototype declarations for methods defined further below
bool isPrime(const long);
bool isAngular(const long);

// Use C++11 (ensures everyone is using correct compiler) using clause
// to define a shortcut to an unordered map. The CountMap is used to
// track the number of times a specific value (key, a long value) was
// encountered.
using CountMap = std::unordered_map<long, int>;

/** Convenience method to track the following information about
    a given number:

    1. Prime numbers are tracked appropriately.
    2. Has a near sinusoidal value.
 */
void compute(const long num, CountMap& tracker) {
#pragma omp parallel
{    
#pragma omp critical
{
    if (isPrime(num)) {
        // Track this number as it is a prime number 
        // and the number of times we have seen it.
        tracker[num]++;
    }
}
#pragma omp critical
{
    if (isAngular(num) && (num % 10 == 1)) {
        tracker[num]++;
    }
}
}
}

//----------------------------------------------------------------------
//   DO  NOT  MODIFY  CODE  BELOW  THIS  LINE
//----------------------------------------------------------------------

bool isPrime(const long num) {
    if ((num % 2) == 0) {
        return false;  // Even numbers are never prime
    }
    for (int fact = 2; (fact <= sqrt(num) + 1); fact++) {
        if (num % fact == 0) {
            return false;  // Found a 2nd factor for the number
        }
    }
    return true;  // the number is prime!
}

bool isAngular(const long num) {
    for (int angle = 0; (angle < 3600); angle++) {
        const long val = 10000 * sin(angle / 10.0);
        if (abs(num - val) < 10) {
            return angle / 10;  // found a close angle
        }
    }
    return -1;  // no valid angle found
}

std::ostream& operator<<(std::ostream& os, const CountMap::const_iterator& it) {
    return (os << it->first << ":" << it->second);
}

int main(int argc, char *argv[]) {
    CountMap tracker;  // Track prime and angle occurrences
    for (int rep = 0; (rep < 5); rep++) {
        for (long num = 1000; (num < 9999); num++) {
            compute(num, tracker);
        }
    }
    // Convert results to sorted list of strings for convenient
    // printing.
    std::vector<std::string> result(tracker.size());
    auto toString = [](CountMap::value_type & i) {
        return std::to_string(i.first) + ":" + std::to_string(i.second);
    };
    std::transform(tracker.begin(), tracker.end(), result.begin(), toString);
    // Sort results to ease validation.
    std::sort(result.begin(), result.end());
    std::copy(result.begin(), result.end(),
            std::ostream_iterator<std::string>(std::cout, "\n"));
    std::cout << "Number of entries: " << result.size() << std::endl;
    return 0;
}

// End of source code
