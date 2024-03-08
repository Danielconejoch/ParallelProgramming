// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0

#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief Generates a random number using rand_r()
 *
 * @param number Pointer to uint64_t
 * @return A cast to void pointer to the generated random number.
 */
void* random_number(void* number);

int main() {
  // Stuff used in the random_number function to generate a random number
  uint64_t random_stuff_1;
  uint64_t random_stuff_2;

  // Pointers to receive the lottery numbers
  uint64_t* random_number_1;
  uint64_t* random_number_2;

  // Create thread nieto1
  pthread_t nieto1;
  int error = pthread_create(&nieto1, NULL, random_number, &random_stuff_1);

  if (error == EXIT_SUCCESS) {
    error = pthread_join(nieto1, (void*)&random_number_1);
    if (error == EXIT_SUCCESS) {
      uint64_t number_print_1 = (uint64_t)random_number_1;
      printf("The number of the grandson 1: %" PRIu64, number_print_1);
      printf("\n");
    } else {
      // Error message if the thread was unable to use join
      fprintf(stderr, "Error: could not join thread\n");
    }
  } else {
    // Error message if the thread was not able to be created
    fprintf(stderr, "Error: could not create thread\n");
  }

  // Create thread nieto2
  pthread_t nieto2;
  error = pthread_create(&nieto2, NULL, random_number, &random_stuff_2);

  if (error == EXIT_SUCCESS) {
    error = pthread_join(nieto2, (void*)&random_number_2);
    if (error == EXIT_SUCCESS) {
      uint64_t number_print_2 = (uint64_t)random_number_2;
      printf("The number of the grandson 2: %" PRIu64, number_print_2);
      printf("\n");
    } else {
      fprintf(stderr, "Error: could not join thread\n");
    }
  } else {
    fprintf(stderr, "Error: could not create thread\n");
  }

  return 0;
}

void* random_number(void* number) {
  uint64_t* my_number = (uint64_t*)number;
  uint64_t number_random;

  // Calculate seed using time, thread ID, and the random stuff
  uint32_t seed = time(NULL) ^ pthread_self() ^ *my_number;

  // Generate random number between 0 and 100
  number_random = rand_r(&seed) % 101;

  // Return the generated random number as a void pointer or fake pointer
  return (void*)number_random;
}
