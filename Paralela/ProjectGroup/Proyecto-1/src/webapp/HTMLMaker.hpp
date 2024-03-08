// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#ifndef HTMLMAKER_HPP
#define HTMLMAKER_HPP

#include <vector>
#include <string>

#include "Consumer.hpp"
#include "NumberStruct.hpp"

/**
 * @class HTMLMaker
 * @brief A class that generates HTML pages based on input data.
 * 
 * This class inherits from Consumer<std::vector<num_t>> and implements the consume method.
 * It also has methods to create and update HTML pages with data.
 */
class HTMLMaker : public Consumer<std::vector<num_t>> {
  /**
  * @brief Creates an HTML file with the given data and numbers.
  * 
  * @param data A vector of num_t containing the data to be displayed in the 
	* HTML file.
  * @param numbers A vector of int containing the numbers to be displayed in 
	* the HTML file.
  * @param data_of_numbers A vector of vectors of int containing the data of 
	* the numbers to be displayed in the HTML file.
  */
  void createHTML(std::vector<num_t> data, std::vector<int> numbers,
    std::vector<std::vector<int>> data_of_numbers);

  /**
  * @brief Consumes the given data.
  * 
  * @param data A vector of num_t containing the data to be consumed.
  */
  void consume(std::vector<num_t> data);

  /**
  * @brief Updates the fact page of the HTTP response with the given numbers and factors.
  * 
  * @param httpResponse The HTTP response to update.
  * @param numbers A vector of int containing the numbers to be displayed in 
	* the fact page.
  * @param allFactors A vector of vectors of int containing the factors of the 
	* numbers to be displayed in the fact page.
  */
  bool updateFactPage(HttpResponse& httpResponse, std::vector<int> numbers,
    std::vector<std::vector<int>> allFactors);

  /**
  * @brief Updates the gold page of the HTTP response with the given numbers 
	* and sums.
  * 
  * @param httpResponse The HTTP response to update.
  * @param numbers A vector of int containing the numbers to be displayed in 
	* the gold page.
  * @param allSums A vector of vectors of int containing the sums of the 
	* numbers to be displayed in the gold page.
  */
  bool updateGoldPage(HttpResponse& httpResponse, std::vector<int> numbers,
    std::vector<std::vector<int>> allSums);

/**
 * @brief Runs the HTMLMaker.
 * This function calls the consumeForever() function to start consuming data indefinitely.
 */
  int run() {
    this->consumeForever();
    return EXIT_SUCCESS;
  }
};

#endif
