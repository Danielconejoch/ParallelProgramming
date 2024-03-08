// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#include "criba.hpp"


std::vector<int> Criba::SieveOfEratosthenes(int n) {
    // Create a vector of bools and a vector of ints
    std::vector<bool> primes;
    std::vector<int> foundPrimes;
    primes.push_back(false);
    primes.push_back(false);

    // Convert negatives numbers into positives
    if (n < 0) {
        n *= -1;
    }
    // Initialize all numbers to n
    for (int i = 2; i < n; i++) {
        primes.push_back(true);
    }

    // Applies the Sieve of Eratosthenes algori
    for (int p = 2; p * p <= n; p++) {
        if (primes[p]) {
            for (int i = p * p; i <= n; i += p)
            primes[i] = false;
        }
    }

    // Save prime numbers in vector primes
    for (int i = 0; i < n + 1; i++) {
        if (primes[i]) {
            foundPrimes.push_back(i);
        }
    }
    // Return
    return foundPrimes;
}
