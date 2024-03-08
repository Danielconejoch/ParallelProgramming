// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "simulation.h"
#include "zip_array.h"
#include "zip_info.h"

int main(int argc, char* argv[]) {
  // struct timespec start_time, finish_time;
  // clock_gettime(CLOCK_MONOTONIC, &start_time);
  simulation_t* simulation = calloc(1, sizeof(simulation_t));
  zip_array_t* zip_array =
      (zip_array_t*)calloc(1, sizeof(zip_array_t));  // pointer to an array
  zip_array_init(zip_array);

  char dictionary[100];
  scanf("%s", dictionary);
  // printf("%s\n", dictionary);

  int64_t max_size = 0;
  scanf("%" PRId64, &max_size);
  // printf("%" PRId64, max_size);

  // Leer cantidad de hilos
  size_t num_threads;
  if (argc > 1) {
    // Si el usuario ingresa
    num_threads = atoi(argv[1]);
  } else {
    // si no se asumen los de la maquina
    num_threads = sysconf(_SC_NPROCESSORS_ONLN) - 1;
  }

  // printf("Cantidad de hilos a utilizar: %d\n", num_threads);

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

  simulation_init(simulation, zip_array, dictionary, max_size, num_threads);

  // clock_gettime(CLOCK_MONOTONIC, &finish_time);
  // double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
  //                       (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

  // printf("Execution time: %.9lfs\n", elapsed_time);
  simulation_destroy(simulation);
  zip_array_destroy(zip_array);
  free(zip_array);

  return 0;
}
