// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// thread_shared_data_t
typedef struct  {
  uint64_t next_thread;
  uint64_t thread_count;
  uint64_t delay;
} shared_data_t;

// thread_private_data_t
typedef struct  {
  uint64_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief Prints a message from the secondary thread
 *
 * @param data message to print
 */
void* greet(void* data);

/**
 * @brief Creats threads and make the error check for each creation
 *
 * @param shared_data struct with data shared between threads
 *
 */
int create_threads(shared_data_t* shared_data);

int main() {
  int error = EXIT_SUCCESS;
  
    // how many threads
  uint64_t thread_count;
  printf("Cantidad de threads: \n");
  scanf("%" PRId64, &thread_count);

  //wait time, used only for the constant delay
  uint64_t wait_time;
  printf("Tiempo a esperar: \n");
  scanf("%" PRId64, &wait_time);

  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->next_thread = 0;
    shared_data->thread_count = thread_count;
    shared_data->delay = wait_time;

    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    error = create_threads(shared_data);

    clock_gettime(CLOCK_MONOTONIC, &finish_time);
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
                          (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

    printf("Execution time: %.9lfs\n", elapsed_time);

    free(shared_data);
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    return 12;
  }
  return error;
}  // end procedure

int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  pthread_t* threads =
      (pthread_t*)malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)calloc(
      shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
         ++thread_number) {
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].shared_data = shared_data;
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], NULL, greet,
                            &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
         ++thread_number) {
      pthread_join(threads[thread_number], NULL);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n",
            shared_data->thread_count);
    error = 22;
  }

  return error;
}


void* greet(void* data) {
  assert(data);
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;

  // // Wait until it is my turn
  // while (shared_data->next_thread < private_data->thread_number) {
  //   usleep(shared_data->delay);
  // }

  // Random delayed busy waiting: wait until it is my turn
  while (shared_data->next_thread < private_data->thread_number) {
    uint32_t my_seed = time(NULL) ^ pthread_self();
    const unsigned my_delay = rand_r(&my_seed) % 50;
    usleep(my_delay);
  }

  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n",
         private_data->thread_number, shared_data->thread_count);

  // Allow subsequent thread to do the task
  ++shared_data->next_thread;

  return NULL;
}  


