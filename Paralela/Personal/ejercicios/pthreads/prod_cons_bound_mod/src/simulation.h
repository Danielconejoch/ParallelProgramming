// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <unistd.h>

enum {
  ERR_NOMEM_SHARED = EXIT_FAILURE + 1,
  ERR_NOMEM_BUFFER,
  ERR_NO_ARGS,
  ERR_BUFFER_CAPACITY,
  ERR_ROUND_COUNT,
  ERR_MIN_PROD_DELAY,
  ERR_MAX_PROD_DELAY,
  ERR_MIN_CONS_DELAY,
  ERR_MAX_CONS_DELAY,
  ERR_CREATE_THREAD,
};

typedef struct {
  size_t thread_count;
  size_t buffer_capacity;
  double* buffer;
  size_t rounds;
  useconds_t producer_min_delay;
  useconds_t producer_max_delay;
  useconds_t consumer_min_delay;
  useconds_t consumer_max_delay;

  sem_t can_produce;
  sem_t can_consume;
} shared_data_t;

typedef struct {
  size_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);

int create_threads(shared_data_t* shared_data);

useconds_t random_between(useconds_t min, useconds_t max);