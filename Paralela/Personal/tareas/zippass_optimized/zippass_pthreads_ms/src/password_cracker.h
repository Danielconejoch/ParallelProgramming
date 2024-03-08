// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#ifndef PASSWORD_CRACKER_H
#define PASSWORD_CRACKER_H

#include <math.h>
#include <pthread.h>
#include <unistd.h>

#include "zip_array.h"

typedef struct {
  size_t thread_count;
  char* dictionary;
  size_t password_size;
  zip_array_t* zips;
  pthread_mutex_t can_open_zip;
  bool* is_cracked;
} shared_data_t;

typedef struct {
  size_t start;
  size_t end;
  size_t current_length;
  size_t current_permutation;
  size_t current_zip;
  size_t thread_number;
  zip_t* zip_archive;
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief Read input data and initialize the cracking process.
 *
 * @param zips Pointer to the zip_array_t structure containing ZIP file
 * information.
 * @param dictionary The dictionary of passwords.
 * @param pass_size The maximum password size to check.
 */
void read_input(zip_array_t* zips, char* dictionary, size_t pass_size);

/**
 * @brief Perform a brute force attack to find passwords for ZIP files.
 *
 * @param data Private data for the thread.
 */
void brute_force(void* data);

#endif
