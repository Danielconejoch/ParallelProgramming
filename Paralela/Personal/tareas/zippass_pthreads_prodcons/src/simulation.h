// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#ifndef SIMULATION_H
#define SIMULATION_H

#include <unistd.h>

#include "password_cracker.h"
#include "queue.h"
#include "zip_array.h"
#include "zip_info.h"

typedef struct {
  int64_t thread_count;
  queue_t* password_queue;
  char* dictionary;
  int64_t password_size;
  zip_array_t* zips;
  pthread_mutex_t* can_open_zip;
  uint64_t cracked_count;
  bool* is_cracked;
  bool all_cracked;
} shared_data_t;

typedef struct {
  uint64_t thread_number;
  zip_t* zip_archive;
  shared_data_t* shared_data;
} private_data_t;

typedef struct {
  private_data_t* private_data;
  shared_data_t* shared_data;
} simulation_t;

/**
 * @brief Initializes the simulation with the provided parameters
 *
 * @param simulation A pointer to the simulation data structure
 * @param zips Pointer to an array of zips
 * @param dictionary String that has the possible characters for the password
 * @param password_size The max lenght of the password
 * @param num_threads How many threads
 * @return A pointer to the initialized simulation
 */
simulation_t* simulation_init(simulation_t* simulation, zip_array_t* zips,
                              char* dictionary, uint64_t password_size,
                              size_t num_threads);

/**
 * @brief Runs the simulation to crack zip files.
 *
 * @param simulation A pointer to the simulation initialized
 */
void simulation_run(simulation_t* simulation);

/**
 * @brief Destroys the simulation and releases allocated resources
 * including the password queue, mutexes, private data, and shared data.
 *
 * @param simulation A pointer to the simulation to destroy
 */
void simulation_destroy(simulation_t* simulation);

#endif
