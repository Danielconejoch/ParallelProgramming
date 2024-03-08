// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "password_cracker.h"
#include "zip_array.h"
#include "zip_info.h"

int main() {
  struct timespec start_time, finish_time;
  clock_gettime(CLOCK_MONOTONIC, &start_time);
  zip_array_t* zip_array =
      (zip_array_t*)calloc(1, sizeof(zip_array_t));  // pointer to an array
  zip_array_init(zip_array);

  char dictionary[100];
  scanf("%s", dictionary);
  // printf("%s\n", dictionary);

  int64_t max_size = 0;
  scanf("%" PRId64, &max_size);
  // printf("%" PRId64, max_size);

  char buffer_path[100];
  memset(buffer_path, 0, sizeof(buffer_path));
  while (fgets(buffer_path, sizeof(buffer_path), stdin) != NULL) {
    // Erases \n and changes to \0
    // if line is empty, continue
    if (buffer_path[0] == '\n' || buffer_path[0] == '\0' ||
        buffer_path[0] == ' ') {
      continue;
    }
    buffer_path[strcspn(buffer_path, "\n")] = '\0';

    zip_info_t new_zip_info;  // Creates a pointer to zip_info

    zip_info_init(&new_zip_info, buffer_path, max_size);
    // Initiates the new zip info and appends it to the array
    zip_array_append(zip_array, &new_zip_info);
  }
  for (uint64_t i = 0; i < zip_array->count; i++) {
    brute_force(&zip_array->elements[i], dictionary, max_size);
    print_zip_info(&zip_array->elements[i]);
    zip_info_destroy(&zip_array->elements[i]);
  }

  // Calcula el tiempo transcurrido
  clock_gettime(CLOCK_MONOTONIC, &finish_time);
  double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
                        (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

  printf("Execution time: %.9lfs\n", elapsed_time);
  // pruebas(dictionary, max_size);
  zip_array_destroy(zip_array);
  free(zip_array);
  return 0;
}
