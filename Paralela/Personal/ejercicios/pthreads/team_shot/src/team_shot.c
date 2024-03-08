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
 * @param number Pointer to a uint64_t
 * @return A void pointer to the generated random number.
 */
void* random_number(void* number);

int main() {
  // Stuff used in the random_number function to generate a random number
  uint64_t random_stuff_1;
  uint64_t random_stuff_2;

  // variables that save the best shot per athlete
  double* best_shot_1;
  double* best_shot_2;

  // sum with the shots per team
  double sum_1 = 0.0;
  double sum_2 = 0.0;

  // Members for team
  uint64_t members;
  scanf("%" PRId64, &members);

  if (members % 2 == 0) {
    fprintf(stderr, "Error: athlete count must be odd\n");
    return EXIT_FAILURE;
  }

  for (uint64_t i = 0; i < members; i++) {
    //
    pthread_t team_1;
    random_stuff_1 = random_stuff_1 + i;
    int64_t error =
        pthread_create(&team_1, NULL, random_number, &random_stuff_1);
    // manejo de errores
    if (error == EXIT_SUCCESS) {
      pthread_join(team_1, (void*)&best_shot_1);
      printf("1.%" PRIu64 " best shot : %lfm", i + 1, *best_shot_1);
      printf("\n");
      sum_1 += *best_shot_1;
      free(best_shot_1);  // Free memory
    } else {
      // Error message if the thread was not able to be created
      fprintf(stderr, "Error: could not create thread\n");
    }

    pthread_t team_2;
    random_stuff_2 = random_stuff_2 + i;
    error = pthread_create(&team_2, NULL, random_number, &random_stuff_2);
    // manejo de errores
    if (error == EXIT_SUCCESS) {
      pthread_join(team_2, (void*)&best_shot_2);
      printf("2.%" PRIu64 " best shot : %lfm", i + 1, *best_shot_2);
      printf("\n");
      sum_2 += *best_shot_2;
      free(best_shot_2);  // Free memory
    } else {
      // Error message if the thread was not able to be created
      fprintf(stderr, "Error: could not create thread\n");
    }
  }

  if (sum_1 > sum_2) {
    printf("Result 1:2, team 1 wins\n");
  } else {
    printf("Result 1:2, team 2 wins\n");
  }
}

void* random_number(void* number) {
  uint64_t* my_number = (uint64_t*)number;

  // Allocate memory to store the generated random number
  double* number_random = (double*)malloc(sizeof(double));

  // Calculate seed using time, thread ID, and the random stuff
  uint32_t seed = time(NULL) ^ pthread_self() ^ *my_number;

  // Generate random number between 0 and 25
  *number_random = rand_r(&seed) % 26;

  // Return the generated random number as a void pointer
  return number_random;
}
