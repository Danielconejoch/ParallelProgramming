// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#ifndef CRIBA_H
#define CRIBA_H

#include <stdbool.h>
#include <vector>

class Criba{
 public:
    /**
     * @brief Finds prime numbers up to a given 'n' using the Sieve of Eratosthenes algorithm.
     * 
     * @param n - The upper limit to find prime numbers up to.
     * @return std::vector<int> - A vector containing prime numbers up to 'n'.
     */
    std::vector<int> SieveOfEratosthenes(int n);
};



#endif  // CRIBA_H
