// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

/**
 * @file NumberStruct.hpp
 * @brief Defines the num_t struct and its related functions.
 */

#ifndef NUMBERSTRUCT_HPP
#define NUMBERSTRUCT_HPP

#include <stdbool.h>
#include <vector>

#include "Consumer.hpp"
#include "Producer.hpp"
#include "HttpResponse.hpp"
#include "Structs.hpp"


/**
 * @brief Struct that represents a number and its related information.
 */
class num_t {
 public:
  num_t() {
  }
  num_t(HttpResponse httpResponse, bool fact, int num, int id, int key,
  size_t* amount_of_numbers) {
    this->httpResponse = httpResponse;
    this->fact = fact;
    this->num = num;
    this->id = id;
    this->key = key;
    this->amount_of_numbers = amount_of_numbers;
  }
  ~num_t() {
    }
  HttpResponse httpResponse;  // Pointer to the HTTP response object.
  size_t* amount_of_numbers;  // Pointer to the amount of numbers that have
  // been processed.
  bool fact;  // Flag indicating whether the number is a factorial
  // number or not.
  int num;  // The number itself
  std::vector<int> num_array;  // Vector containing the digits of the number.
  int key;  // Key used for sorting the numbers.
  int id;  // ID used for sorting the numbers.
};

/**
 * @brief Overloads the == operator for num_t structs.
 * @param num1 The first num_t struct.
 * @param num2 The second num_t struct.
 * @return True if the two structs are equal, false otherwise.
 */
bool operator==(num_t num1, num_t num2);

#endif
