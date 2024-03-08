// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0

#include <chrono>
#include <inttypes.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>
#include <memory>

// thread_shared_data_t
struct shared_data_t {
  uint64_t thread_count;
};

// thread_private_data_t
struct private_data_t {
  uint64_t thread_number;
  std::shared_ptr<shared_data_t> shared_data;
};

/**
 * @brief Creates and executes threads based on the provided shared data.
 *
 * @param shared_data A shared pointer to the shared data structure.
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int create_threads(std::shared_ptr<shared_data_t> shared_data);

/**
 * @brief Greets from a secondary thread, printing the thread number and the total number of threads.
 *
 * @param data A pointer to the private data structure for the thread.
 */
void greet(private_data_t* data);