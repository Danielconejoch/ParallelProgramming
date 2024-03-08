// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#include "Factoring.hpp"

#include <iostream>
#include <vector>

using std::vector;

vector<int> Factoring::primeFact(vector<int> primes, int numTF) {
    // Create a vector to store factorized numbers
    vector<int> factorizedNumber;
    if (primes.size() > 0) {
        int* B = new int[primes.size()];

        // Initialize the B array to zeros
        for (size_t i = 0; i < primes.size(); i++) {
            B[i] = 0;
        }
        // Factorize 'numTF' using prime numbers
        for (size_t i = 0; i < primes.size() && numTF != 1; i++) {
            if (numTF > 0) {
                while (numTF % primes[i] == 0) {
                    numTF = numTF/primes[i];
                    B[i]++;
                }
            }
        }
        // Store the prime factors and their exponents in 'factorizedNumber'
        for (size_t i = 0; i < primes.size(); i++) {
            if (B[i] >= 1) {
                factorizedNumber.push_back(primes[i]);
                factorizedNumber.push_back(B[i]);
            }
        }
        // Free memory
        delete[] B;
    } else {
        // If there are no prime numbers, adds -1 to the end of the vector
        factorizedNumber.push_back(-1);
    }
    // Return
    return factorizedNumber;
}

vector<vector<int>> Factoring::primeFactAllNumbers(vector<int> numbers) {
    // Calculate the greatest number of the vector numbers
    int greatestNumber = 0;
    for (size_t i = 0; i < numbers.size(); i++) {
        if (greatestNumber < numbers[i]) {
            greatestNumber = numbers[i];
        }
    }
    // Create an instance of the Criba class
    Criba criba;

    // Find prime numbers up to 'greatestNumber'
    vector<int> primes = criba.SieveOfEratosthenes(greatestNumber);
    vector<vector<int>> allNumbersFactorized;
    vector<int> notFactorizable;

    // Factorize each number in the "numbers" vector
    for (size_t i = 0; i < numbers.size(); i++) {
        notFactorizable = Factoring::primeFact(primes, numbers[i]);
        allNumbersFactorized.push_back(notFactorizable);
    }
    // Return
    return allNumbersFactorized;
}
