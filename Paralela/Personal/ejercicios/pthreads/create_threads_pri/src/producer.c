// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#include "producer.h"

#include <stdio.h>

#include "common.h"

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)(private_data->shared_data);

  while (true) {
    // declare my_unit := 0
    size_t my_unit = 0;
    // lock(can_access_next_unit)
    pthread_mutex_lock(&shared_data->can_access_next_unit);
    // If is there pending work, take a unit for producing
    if (shared_data->next_unit < shared_data->unit_count) {
      my_unit = ++shared_data->next_unit;
    } else {
      // unlock(can_access_next_unit)
      pthread_mutex_unlock(&shared_data->can_access_next_unit);
      // break while
      break;
    }
    // unlock(can_access_next_unit)
    pthread_mutex_unlock(&shared_data->can_access_next_unit);

    usleep(1000 * random_between(shared_data->producer_min_delay,
                                 shared_data->producer_max_delay));
    queue_enqueue(&shared_data->queue, my_unit);
    printf("Produced %zu the thread number: %" PRId64 "\n", my_unit,
           private_data->thread_number);

    // signal(can_consume)
    sem_post(&shared_data->can_consume);
  }

  return NULL;
}