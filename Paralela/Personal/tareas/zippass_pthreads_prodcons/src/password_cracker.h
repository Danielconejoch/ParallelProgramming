// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#ifndef PASSWORD_CRACKER_H
#define PASSWORD_CRACKER_H

#include <assert.h>

#include "zip_array.h"

/**
 * @brief Generates and enqueues password permutations to crack the password
 *
 * @param data A pointer to shared data containing simulation parameters
 */

void brute_force(void* data);

/**
 * @brief Tries to decrypt ZIP files using passwords from a queue
 *
 * @param data A pointer to the thread's private data
 */
void consume_password(void* data);
#endif
