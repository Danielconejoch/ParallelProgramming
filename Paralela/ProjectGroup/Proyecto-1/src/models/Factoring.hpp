// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#ifndef FACTORING_H
#define FACTORING_H

#include <vector>

#include "criba.hpp"

using std::vector;
/**
 * @brief Class that provides functions related to prime number factorization.
 */
class Factoring{
 public:
    /**
     * @brief Calculate the prime factorization of an integer.
     *
     * @param primes A vector of prime numbers.
     * @param numTF The number to factorize.
     * @return A vector containing prime factors and their exponents.
     */
    vector<int> primeFact(vector<int> primes, int numTF);

    /**
     * @brief ...
     *
     * @param numbers A vector of integers.
     * @return A vector of vectors containing the ptimr factorizations of the numbers.
     */
    vector<vector<int>> primeFactAllNumbers(vector<int> numbers);
};

#endif  // FACTORING_H

