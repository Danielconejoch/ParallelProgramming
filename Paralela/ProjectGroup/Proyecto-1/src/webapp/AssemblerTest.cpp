// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo
// Universidad de Costa Rica

#include "AssemblerTest.hpp"

#include <iostream>

bool AssemblerTest::check_if_request_completed(int key) {
  // Retrieve the vector of 'num_t' elements associated with the provided key
  std::vector<num_t> numeros_de_usuario = this->numeros[key];
  // Compare the size of the vector with the expected amount of numbers
  return numeros_de_usuario.size() ==
  *(numeros_de_usuario[0].amount_of_numbers);
}

void AssemblerTest::insert_data(num_t data) {
  // Check if the key exists in the collection
  const auto& itr = this->numeros.find(data.key);
  if ( itr == this->numeros.end() ) {
    // If the key doesn't exist, create a new vector for that key
    std::vector<num_t> new_vec;
    this->numeros[data.key] = new_vec;
  }
  // Retrieve the vector associated with the key
  std::vector<num_t> numeros_de_usuario = this->numeros[data.key];

  // Iterate through the vector to find the correct position to insert 'data'
  // based on 'id'
  for (size_t i = 0; i < numeros_de_usuario.size(); ++i) {
    if (numeros_de_usuario[i].id > data.id) {
      // Insert 'data' at the appropriate position and return
      this->numeros[data.key].insert(this->numeros[data.key].begin() + i, data);
      return;
    }
  }
  // If 'id' is greater than all existing 'id's, simply push back 'data' to the
  // vector
  this->numeros[data.key].push_back(data);
}

void AssemblerTest::consume(num_t data) {
  // Insert 'data' into the internal collection
  insert_data(data);
  // Check if the request associated with 'data.key' is completed
  if (check_if_request_completed(data.key)) {
    // If completed, produce results
    produce(this->numeros[data.key]);
  }
}

int AssemblerTest::run() {
  // Initiate continuous data consumption
  this->consumeForever();
  // Return the exit status indicating successful completion
  return EXIT_SUCCESS;
}
