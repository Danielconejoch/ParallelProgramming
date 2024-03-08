// Copyright 2023 Alberto Rojas <alberto.rojassalazar@ucr.ac.cr> CC-BY 4.0
#include "queue.h"

#include <stdio.h>

void queue_init(queue_t* queue) {
  queue->head = NULL;
  queue->tail = NULL;
  queue->can_consume = (sem_t*)calloc(1, sizeof(sem_t));
  sem_init(queue->can_consume, 0, 0);
  queue->can_modify = (pthread_mutex_t*)calloc(1, sizeof(pthread_mutex_t));
  pthread_mutex_init(queue->can_modify, NULL);
}

void queue_enqueue(queue_t* queue, char* text) {
  element_t* element = (element_t*)calloc(1, sizeof(element_t));
  element->text = text;
  element->next = NULL;

  pthread_mutex_lock(queue->can_modify);
  if (queue_is_empty(queue)) {  // cola vacia
    queue->head = element;
    queue->tail = element;
  } else {  // cola con elementos
    queue->tail->next = element;
    queue->tail = element;
  }
  pthread_mutex_unlock(queue->can_modify);
  sem_post(queue->can_consume);
}

char* queue_dequeue(queue_t* queue) {
  sem_wait(queue->can_consume);
  pthread_mutex_lock(queue->can_modify);
  if (!queue_is_empty(queue)) {
    element_t* element = queue->head;
    if (queue->head->next == NULL) {
      queue->head = NULL;
      queue->tail = NULL;
    } else {
      queue->head = element->next;
    }
    pthread_mutex_unlock(queue->can_modify);
    char* text = element->text;
    free(element);
    return text;
  }
  pthread_mutex_unlock(queue->can_modify);
  return NULL;
}

bool queue_is_empty(queue_t* queue) {
  if (queue->head == NULL && queue->tail == NULL) {
    return true;
  }
  return false;
}

void queue_destroy(queue_t* queue) {
  while (!queue_is_empty(queue)) {
    // printf("estoy liberando");
    char* tmp = queue_dequeue(queue);
    free(tmp);
  }
  sem_destroy(queue->can_consume);
  free(queue->can_consume);
  pthread_mutex_destroy(queue->can_modify);
  free(queue->can_modify);
}
