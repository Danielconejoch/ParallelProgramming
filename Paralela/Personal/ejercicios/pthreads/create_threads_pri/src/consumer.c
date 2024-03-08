// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#include "consumer.h"

#include <stdbool.h>
#include <stdio.h>

#include "common.h"

void* consume(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)(private_data->shared_data);

  while (true) {
    // lock(can_access_consumed_count)
    pthread_mutex_lock(&shared_data->can_access_consumed_count);
    if (shared_data->consumed_count < shared_data->unit_count) {
      // Reserve the next product to me
      ++shared_data->consumed_count;
    } else {
      // unlock(can_access_consumed_count)
      pthread_mutex_unlock(&shared_data->can_access_consumed_count);
      // break while
      break;
    }
    // unlock(can_access_consumed_count)
    pthread_mutex_unlock(&shared_data->can_access_consumed_count);

    // wait(can_consume)
    sem_wait(&shared_data->can_consume);

    size_t value = 0;
    queue_dequeue(&shared_data->queue, &value);
    printf("\tConsuming %zu the thread number: %" PRId64 "\n", value,
           private_data->thread_number);
    usleep(1000 * random_between(shared_data->consumer_min_delay,
                                 shared_data->consumer_max_delay));
  }

  return NULL;
}