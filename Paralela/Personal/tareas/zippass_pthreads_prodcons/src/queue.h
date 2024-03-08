// Copyright 2023 Alberto Rojas <alberto.rojassalazar@ucr.ac.cr> CC-BY 4.0
#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct element {
  char* text;
  struct element* next;
} element_t;

typedef struct queue {
  element_t* head;
  element_t* tail;
  sem_t* can_consume;
  pthread_mutex_t* can_modify;
} queue_t;

void queue_init(queue_t* queue);
void queue_enqueue(queue_t* queue, char* text);
char* queue_dequeue(queue_t* queue);
void queue_destroy(queue_t* queue);
bool queue_is_empty(queue_t* queue);

#endif
