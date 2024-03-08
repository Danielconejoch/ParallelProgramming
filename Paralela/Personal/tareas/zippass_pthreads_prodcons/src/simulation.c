// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0

#include "simulation.h"

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Creates producer and consumer threads for password cracking. Producer
 * threads generate password permutations, and consumer threads attempt to crack
 * zip files using these passwords.
 *
 * @param simulation A pointer to the simulation initialized
 */
void create_producers_consumers(simulation_t* simulation);

simulation_t* simulation_init(simulation_t* simulation, zip_array_t* zips,
                              char* dictionary, uint64_t password_size,
                              size_t num_threads) {
  // inicializa shared data y sus parametros
  simulation->shared_data = calloc(1, sizeof(shared_data_t));
  // cantidad de hilos
  simulation->shared_data->thread_count = num_threads;
  // array de zips
  simulation->shared_data->zips = zips;
  // caracteres que se usan para la contrasena
  simulation->shared_data->dictionary = dictionary;
  // max size de la contrasena
  simulation->shared_data->password_size = password_size;
  // alloca memoria mutex
  simulation->shared_data->can_open_zip =
      (pthread_mutex_t*)calloc(1, sizeof(pthread_mutex_t));
  // array de bool que dice cuales zips ya tienen contrasena
  simulation->shared_data->is_cracked =
      (bool*)calloc(simulation->shared_data->zips->count, sizeof(bool));
  // bool que indica que todos tiene contrasena
  simulation->shared_data->all_cracked = false;
  // inicializa mutex
  pthread_mutex_init(simulation->shared_data->can_open_zip, NULL);
  // inicializa private data
  simulation->private_data =
      calloc(simulation->shared_data->thread_count, sizeof(private_data_t));

  // llama al metodo run para crear los threads y empezar el programa
  simulation_run(simulation);

  return simulation;
}

void simulation_run(simulation_t* simulation) {
  // inicia prodcons que posteriormente crackea contarsenas
  create_producers_consumers(simulation);

  // imprime el zip con su respectiva contrasena y luego libera memoria
  for (uint64_t i = 0; i < simulation->shared_data->zips->count; i++) {
    print_zip_info(&simulation->shared_data->zips->elements[i]);
    zip_info_destroy(&simulation->shared_data->zips->elements[i]);
  }
}

void simulation_destroy(simulation_t* simulation) {
  assert(simulation);
  // libera memoria de toda la simulacion, el queue y la memoria privada y
  // compartida
  queue_destroy(simulation->shared_data->password_queue);
  pthread_mutex_destroy(simulation->shared_data->can_open_zip);
  free(simulation->shared_data->can_open_zip);
  free(simulation->shared_data->password_queue);
  free(simulation->shared_data->is_cracked);
  free(simulation->private_data);
  free(simulation->shared_data);
  free(simulation);
}

void create_producers_consumers(simulation_t* simulation) {
  // shared data
  shared_data_t* shared_data = (shared_data_t*)(simulation->shared_data);

  // crea el queue para las contrasenas
  shared_data->password_queue = (queue_t*)calloc(1, sizeof(queue_t));
  queue_init(simulation->shared_data->password_queue);

  // create producer
  pthread_t producer;

  pthread_create(&producer, NULL, (void*)brute_force,
                 (void*)simulation->shared_data);

  // guarda la cantidad de consumidores en una variable
  uint64_t consumer_count = shared_data->thread_count;

  // private data
  private_data_t* private_data = (private_data_t*)(simulation->private_data);

  // guarda espacio para los threads
  pthread_t* consumers = (pthread_t*)calloc(consumer_count, sizeof(pthread_t));

  // crea los consumidores
  for (uint64_t consumer_number = 0; consumer_number < consumer_count;
       ++consumer_number) {
    private_data[consumer_number].shared_data = shared_data;
    private_data[consumer_number].thread_number = consumer_number;
    pthread_create(&consumers[consumer_number], NULL, (void*)consume_password,
                   &private_data[consumer_number]);
  }

  // join producer
  pthread_join(producer, NULL);

  // join consumers
  for (uint64_t consumer_number = 0; consumer_number < consumer_count;
       ++consumer_number) {
    pthread_join(consumers[consumer_number], NULL);
  }
  free(consumers);
}
