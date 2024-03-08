// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  double duration;
  double speedup;
  double efficiency;
} mapping_t;

typedef struct {
  mapping_t* mapping;
  size_t thread_count;
  size_t* numbers_array;
  size_t array_size;
  size_t work_amount;
  pthread_barrier_t* mapping_ready;
} shared_data_t;

typedef struct {
  size_t thread_num;
  size_t* mapping_results;
  shared_data_t* shared_data;
} private_data_t;

void read_arguments(shared_data_t* shared_data);

void mapping_simulation(void* data);

void block_mapping(void* data);

void cyclic_mapping(void* data);

void dynamic_mapping(void* data);

void print_mapping();

int main(int argc, char const* argv[]) {
  // iniciar shared data
  shared_data_t* shared_data = calloc(1, sizeof(shared_data_t));

  // Leer cantidad de hilos
  if (argc > 1) {
    // Si el usuario ingresa
    shared_data->thread_count = atoi(argv[1]);
  } else {
    // si no se asumen los de la maquina
    shared_data->thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  }

  // Leer numeros para el mapeo
  read_arguments(&shared_data);

  // TODO iniciar semaforos y barrera

  // Guardar memoria para los threads
  pthread_t* threads =
      (pthread_t*)calloc(shared_data->thread_count, sizeof(pthread_t));

  // iniciar private data
  private_data_t* private_data =
      calloc(shared_data->thread_count, sizeof(private_data_t));

  for (size_t i = 0; i < shared_data->thread_count; i++) {
    // puntero a memoria compartida
    private_data[i].shared_data = shared_data;
    private_data[i].thread_num = i;
    private_data[i].mapping_results =
        calloc(shared_data->array_size, sizeof(size_t));
    // Create threads
    pthread_create(&threads[i], NULL, (void*)mapping_simulation,
                   &private_data[i]);
  }

  // join

  for (size_t i = 0; i < shared_data->thread_count; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}

void read_arguments(shared_data_t* shared_data) {
  printf("Ingrese una secuencia de números (separados por espacios):\n");

  size_t* numbers = NULL;
  size_t size = 0;

  size_t number;
  size_t number_count;
  while (scanf("%d", &number) == 1) {
    numbers = (size_t*)realloc(numbers, (size + 1) * sizeof(size_t));
    numbers[size] = number;
    number_count += number;
    size++;
  }
  // asigna datos al shared data
  shared_data->numbers_array = numbers;
  shared_data->array_size = size;
  shared_data->work_amount = number_count;

  // crea la barrera
  pthread_barrier_init(shared_data->mapping_ready, NULL,
                       shared_data->thread_count);
}

void mapping_simulation(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;

  block_mapping(private_data);
  cyclic_mapping(private_data);
  dynamic_mapping(private_data);
  print_mapping(private_data);
}

void block_mapping(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;
  // calculate the start
  size_t start =
      static_mapping(private_data->thread_num, shared_data->work_amount,
                     shared_data->thread_count);
  // calculate the end
  size_t end =
      static_mapping(private_data->thread_num + 1, shared_data->work_amount,
                     shared_data->thread_count);

  for (size_t start; start < end; start++) {
    // guarda los el resultado en la posicion 1 del vector
    private_data->mapping_results[0] += shared_data->numbers_array[start];
  }

  pthread_barrier_wait(shared_data->mapping_ready);
}

void cyclic_mapping(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;

  // hace un salto por la cantidad de threads que hay
  for (size_t i = private_data->thread_num; i < shared_data->array_size;
       i += shared_data->thread_count) {
    // guarda los el resultado en la posicion 1 del vector
    private_data[i].mapping_results[1] += shared_data->numbers_array[i];
  }

  pthread_barrier_wait(shared_data->mapping_ready);
}

void dynamic_mapping(void* data) {}

int static_mapping(size_t thread_num, size_t max_permutation,
                   size_t thread_count) {
  size_t division_result = max_permutation / thread_count;
  size_t remainder = max_permutation % thread_count;

  if (thread_num < remainder) {
    return thread_num * (division_result + 1);
  } else {
    return thread_num * division_result + remainder;
  }
}