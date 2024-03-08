// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#include "goldbach.hpp"

using std::vector;

bool Goldbach::bruteForceFindNumber(vector<int> A, int numTF) {
    for (size_t i = 0; i < A.size(); i++) {
        // Check if 'numTF' is in the 'A' vector using brute force
        if (A[i] == numTF) {
            return true;
        }
    }
    return false;
}

bool Goldbach::findNumber(vector<int> A, int numTF) {
    // Chechk if the size of the array is 0, if it is, return 'false'
    // indicating an error
    if (A.size() == 0) {
        return false;
    }
    // Initialize values
    size_t start = 0;
    size_t end = A.size() - 1;
    size_t midPos = end/2;
    while (true) {
        // Check if 'numTF' is in the 'A' vector using binary search
        if (start == midPos) {
            return numTF == A[end] || (midPos < A.size()
                && numTF == A[midPos]);
        }
        if (numTF < A[midPos]) {
            end = midPos;
        } else if (numTF > A[midPos]) {
            start = midPos;
        } else {
            return true;
        }
        midPos = (start + end)/2;
    }
}
vector<int> Goldbach::primeSum(vector<int> primes, int num) {
    // Create a vector to store the sums
    vector<int> sumas;
    // Calculates de absolute value of "num"
    num = abs(num);
    // Check if the number that is going to be calculated is greater than 0
    if (num != 0) {
        // Check if the number is even
        if (num % 2 == 0) {
            for (size_t i = 0; i < primes.size()/2 + 1 && primes[i] <= num;
                i++) {
                // Check if the sum of two prime numbers results in 'num'
                if (Goldbach::findNumber(primes, num - primes[i]) &&
                    !Goldbach::bruteForceFindNumber(sumas, num - primes[i])) {
                    sumas.push_back(primes[i]);
                    sumas.push_back(num - primes[i]);
                }
            }
        // If the number is odd
        } else {
            for (size_t i = 0; i < primes.size() && primes[i] <= num; i++) {
                for (size_t j = i; j < primes.size()/2 + 1
                    && primes[i] + primes[j] <= num; j++) {
                    // Check if the sum of three prime numbers results in 'num'
                    if (Goldbach::findNumber(primes,
                        num - primes[i] - primes[j]) &&
                            !Goldbach::bruteForceFindNumber(sumas,
                                num - primes[i] - primes[j])) {
                        sumas.push_back(primes[i]);
                        sumas.push_back(primes[j]);
                        sumas.push_back(num - primes[i] - primes[j]);
                    }
                }
            }
        }
    }

    if (sumas.size() == 0) {
        sumas.push_back(0);
    }
    // Return
    return sumas;
}

vector<vector<int>> Goldbach::goldbachAllNumbers(vector<int> numbers) {
    // Find the greatest number of the array "numbers"
    int greatestNumber = 0;
    for (size_t i = 0; i < numbers.size(); i++) {
        if (greatestNumber < abs(numbers[i])) {
            greatestNumber = abs(numbers[i]);
        }
    }
    // Create an instance of the 'Criba' class
    Criba criba;

    // Find prime numbers up to 'greatestNumber'
    vector<int> primes = criba.SieveOfEratosthenes(greatestNumber);
    vector<vector<int>> allNumbersWithGoldbach;
    vector<int> amountOfSums;
    for (size_t i = 0; i < numbers.size(); i++) {
        // Calculate Goldbach sums for each number in 'numbers'
        allNumbersWithGoldbach.push_back(Goldbach::primeSum(primes,
            numbers[i]));
    }
    // Return
    return allNumbersWithGoldbach;
}
