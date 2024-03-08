// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#ifndef ASSEMBLERTEST_HPP
#define ASSEMBLERTEST_HPP

#include <map>
#include <vector>

#include "NumberStruct.hpp"
#include "Assembler.hpp"

/**
 * @file AssemblerTest.hpp
 * @brief This file contains the implementation of the AssemblerTest class, which is a subclass of Assembler.
 * It is used to assemble a vector of num_t objects from a stream of num_t objects.
 */

/**
 * @brief AssemblerTest class is a subclass of Assembler.
 * It is used to assemble a vector of num_t objects from a stream of num_t objects.
 */
class AssemblerTest : public Assembler<num_t, std::vector<num_t>> {
  using Assembler::Assembler;
  std::map<int, std::vector<num_t>> numeros;  // A map that stores the numbers
  // entered by the user.

  /**
  * @brief Checks if the request for a given key has been completed.
  * @param key The key for which the request is being checked.
  * @return True if the request has been completed, false otherwise.
  */
  bool check_if_request_completed(int key);

	/**
 	* @brief Inserts data into a map of vectors based on key values.
 	* 
 	* This function inserts data into a map of vectors based on the key value 
	* provided in the 'data' argument.
 	* If the key does not exist, a new vector is created and the data is inserted 
	* into it.
 	* If the key exists, the data is inserted into the existing vector in a 
	* sorted manner based on 'id' values.
 	* 
 	* @param data The data of type 'num_t' to be inserted.
 	*/
  void insert_data(num_t data);

  /**
  * @brief Consumes a num_t object and adds it to the vector of numbers for the 
	* corresponding key.
  * If the request for that key has been completed, the vector is produced.
  * @param data The num_t object to be consumed.
  */
  void consume(num_t data);

  int run();
};

#endif
