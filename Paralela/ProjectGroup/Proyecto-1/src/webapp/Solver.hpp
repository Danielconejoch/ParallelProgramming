// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>

#include "Assembler.hpp"
#include "NumberStruct.hpp"

/**
 * @class Solver
 * 
 * This class inherits from the Assembler class and provides additional
 *  functionality
 * for solving the problem. It maintains a list of prime numbers and the 
 * largest number encountered during the solving process.
 */
class Solver : public Assembler<num_t, num_t> {
  /// Objects of this class cannot be copied

  using Assembler::Assembler;

 public:
  std::vector<int> prime_numbers; /**< Vector containing the prime numbers. */
  int largest_number = 0;  // The largest number encountered during the
  // solving process.

  /**
  * @brief Executes the Solver by continuously consuming resources until 
  * completion.
  * 
  * This function runs the Solver by invoking the 'consumeForever()' method.
  * It continues consuming resources indefinitely until completion and then 
  * returns an exit status.
  * 
  * @return An integer representing the exit status of the Solver (EXIT_SUCCESS 
  * if successful).
  */
  int run() override;

  /**
 * @brief Consumes data and performs operations based on its attributes.
 * 
 * This function consumes the provided 'num_t' data and performs various 
 * operations based on its attributes.
 * It checks if the 'data.num' is larger than the 'largest_number' and updates 
 * the 'largest_number' accordingly.
 * It then uses different algorithms based on the 'data.fact' attribute to 
 * compute 'num_array'.
 * Finally, it produces the processed 'data'.
 * 
 * @param data The 'num_t' data to be consumed and processed.
 */
  void consume(num_t data);

  /// Destructor
  virtual ~Solver() noexcept {
  }
};

#endif
