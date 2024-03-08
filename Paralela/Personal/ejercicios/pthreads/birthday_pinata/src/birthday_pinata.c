// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  uint64_t remaining_hits;
  bool broken;
  pthread_mutex_t can_access_position;
  uint64_t thread_count;
} shared_data_t;

typedef struct {
  uint64_t thread_number;  //
  uint64_t hits;
  bool broke_pinata;
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief Hits the pinata decreasing its remaining hits and increasing the hit
 * per thread
 *
 * @param data thread number
 */
void* hit_pinata(void* data);

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
  scanf("%" PRId64, &thread_count);

  // how many hits can get pinata
  uint64_t hits_pinata;
  scanf("%" PRId64, &hits_pinata);

  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->remaining_hits = hits_pinata;
    shared_data->broken = false;
    error = pthread_mutex_init(&shared_data->can_access_position, NULL);
    if (error == EXIT_SUCCESS) {
      shared_data->thread_count = thread_count;
      error = create_threads(shared_data);
      pthread_mutex_destroy(&shared_data->can_access_position);
      free(shared_data);
    } else {
      fprintf(stderr, "Error: could not init mutex\n");
      return EXIT_FAILURE;
    }
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    return EXIT_FAILURE;
  }
  return error;
}

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
      private_data[thread_number].hits = 0;
      private_data[thread_number].broke_pinata = false;
      private_data[thread_number].shared_data = shared_data;
      error = pthread_create(&threads[thread_number], NULL, hit_pinata,
                             &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = EXIT_FAILURE;
        break;
      }
    }

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
         ++thread_number) {
      pthread_join(threads[thread_number], NULL);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n",
            shared_data->thread_count);
    error = EXIT_FAILURE;
  }

  return error;
}

void* hit_pinata(void* data) {
  assert(data);
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;

  while (true) {
    // lock(can access position)
    pthread_mutex_lock(&shared_data->can_access_position);
    if (shared_data->broken == false) {
      // pinata has got a hit
      shared_data->remaining_hits--;
      // Thread incrases one hit
      private_data->hits++;
      // condition, if the pinata breaks, different print
      if (shared_data->remaining_hits == 0) {
        shared_data->broken = true;
        printf("Thread %" PRIu64 "/%" PRIu64 ": %" PRIu64
               " hits, I broke the pinata\n",
               private_data->thread_number + 1, shared_data->thread_count,
               private_data->hits);
        private_data->broke_pinata = true;
        // unlock(can_access_position)
        pthread_mutex_unlock(&shared_data->can_access_position);
      }
    } else {
      // unlock(can_access_position)
      pthread_mutex_unlock(&shared_data->can_access_position);
      break;
    }
    pthread_mutex_unlock(&shared_data->can_access_position);
    //usleep(1);
  }

  if (private_data->broke_pinata == false) {
    printf("Thread %" PRIu64 "/%" PRIu64 ": %" PRIu64 " hits\n",
           private_data->thread_number + 1, shared_data->thread_count,
           private_data->hits);
  }
  return NULL;
}
