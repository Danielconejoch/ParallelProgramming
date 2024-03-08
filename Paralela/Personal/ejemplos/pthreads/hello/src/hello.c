// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Prints a message from the secondary thread
 *
 * @param data message to print
 */
void* greet(void* data);

int main() {
  // create a thread
  pthread_t thread;
  int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ NULL);
  if (error == EXIT_SUCCESS) {
    // print "Hello from main thread"
    // usleep(1);  // indeterminism
    printf("Hello from main thread\n");
    pthread_join(thread, /*value_ptr*/ NULL);
  } else {
    // error message if the thread was not able to be created
    fprintf(stderr, "Error: could not create secondary thread\n");
  }
  return error;
}

void* greet(void* data) {
  (void)data;
  // print "Hello from secondary thread"
  printf("Hello from secondary thread\n");
  return NULL;
}