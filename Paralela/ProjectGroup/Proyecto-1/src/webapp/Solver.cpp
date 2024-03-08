// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#include <iostream>

#include "Solver.hpp"
#include "Factoring.hpp"
#include "goldbach.hpp"

void Solver::consume(num_t data) {
  // Check if the provided 'data.num' is larger than the current
  // 'largest_number'
  if (largest_number < data.num) {
    // If so, update 'largest_number' and compute prime numbers
    // up to 'data.num'
    Criba criba;
    this->largest_number = data.num;
    this->prime_numbers = criba.SieveOfEratosthenes(data.num);
  }
  // Check if the 'data.fact' flag is set
  if (data.fact) {
    // If 'fact' is true, perform prime factorization
    Factoring factor;
    data.num_array = factor.primeFact(this->prime_numbers, data.num);
  } else {
    // If 'fact' is false, perform Goldbach's conjecture computation
    Goldbach gold;
    data.num_array = gold.primeSum(this->prime_numbers, data.num);
  }
  // Produce the processed 'data'
  produce(data);
}

int Solver::run() {
  // Invoke the 'consumeForever()' method to execute the Solver
  this->consumeForever();
  // Return the exit status indicating successful completion
  return EXIT_SUCCESS;
}
