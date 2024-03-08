// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#ifndef GOLBACH_H
#define GOLBACH_H

#include <stdbool.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "criba.hpp"

using std::vector;
// TODO(Dariel): Optimizar codigo

/**
 * @brief Class that provides functions related to the Goldbach conjecture.
 */
class Goldbach {
 public:
    /**
     * @brief Search for a number in a vector using a brute force.
     *
     * @param A A vector of integers.
     * @param numTF The number to search for.
     * @return "true" if the number is found in the vector, otherwise return
     * "false".
     */
    bool bruteForceFindNumber(vector<int> A, int numTF);
    /**
     * @brief Search for a number in a vector using binary search.
     *
     * @param A A vector of integers.
     * @param numTF The number to search for.
     * @return "true" if the number is found in the vector, otherwise return 
     * "false".
     */
    bool findNumber(vector<int> A, int numTF);

    /**
     * @brief Find all the prime sums to a given numbers.
     *
     * @param primes A vector of prime numbers.
     * @param num The integer for which prime sums are.
     * @return A vector containing the prime sums found.
     */
    vector<int> primeSum(vector<int> primes, int num);

    /**
     * @brief Contains all vectors with the goldbach sums for each number
     *
     * @param numbers A vector of integers.
     * @return A vector of vectors containing the goldbach sums for each number
     */
    vector<vector<int>> goldbachAllNumbers(vector<int> numbers);
};

#endif  // GOLBACH_H
