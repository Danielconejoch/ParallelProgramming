// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#define PASSWORD_CRACKED 1
#define PASSWORD_NOT_CRACKED 0

#include "password_cracker.h"

/**
 * @brief Attempt to crack a password for a ZIP file.
 *
 * @param zip Pointer to the zip_info_t structure for the ZIP file.
 * @param password The password to attempt.
 * @param private_data Private data for the thread.
 * @param zip_number The number of the ZIP file being processed.
 * @return PASSWORD_CRACKED (1) if the password is found, PASSWORD_NOT_CRACKED
 * (0) otherwise.
 */
int try_password(zip_info_t* zip, char* password, private_data_t* private_data,
                 int64_t zip_number);

/**
 * @brief Generate passwords for the brute force attack.
 *
 * @param data Private data for the thread.
 * @return PASSWORD_CRACKED (1) if the password is found, PASSWORD_NOT_CRACKED
 * (0) otherwise.
 */
int generate_password(void* data);

/**
 * @brief Perform static mapping for a thread.
 *
 * @param thread_num Thread number.
 * @param max_permutation Maximum permutation value.
 * @param thread_count Total number of threads.
 * @return Calculated mapping value.
 */
int static_mapping(size_t thread_num, size_t max_permutation,
                   size_t thread_count);

void read_input(zip_array_t* zips, char* dictionary, size_t pass_size) {
  // Inicializar datos compartidos y sus respectivos atributos
  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  shared_data->thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  shared_data->dictionary = dictionary;
  shared_data->password_size = pass_size;
  shared_data->zips = zips;
  shared_data->is_cracked =
      (bool*)calloc(shared_data->zips->count, sizeof(bool));

  pthread_mutex_init(&shared_data->can_open_zip, NULL);

  // Iniciar datos privados
  private_data_t* private_data = (private_data_t*)calloc(
      shared_data->thread_count, sizeof(private_data_t));

  for (size_t thread_num = 0; thread_num < shared_data->thread_count;
       thread_num++) {
    private_data[thread_num].shared_data = shared_data;
    private_data[thread_num].thread_number = thread_num;
  }
  for (size_t zip_count = 0; zip_count < shared_data->zips->count;
       zip_count++) {
    for (size_t thread_num = 0; thread_num < shared_data->thread_count;
         thread_num++) {
      private_data[thread_num].current_zip = zip_count;
    }
    brute_force(private_data);
  }

  free(private_data);
  free(shared_data->is_cracked);
  pthread_mutex_destroy(&shared_data->can_open_zip);
  free(shared_data);
}

void brute_force(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)private_data->shared_data;
  size_t dictionary_length = strlen(shared_data->dictionary);
  size_t max_permutation = 1;
  int errorp = 0;
  zip_info_t* zip = &shared_data->zips->elements[private_data->current_zip];

  // Guardar memoria para los threads
  pthread_t* threads =
      (pthread_t*)calloc(shared_data->thread_count, sizeof(pthread_t));
  // Recorrer en largo de la contrasena
  for (size_t pass_length = 1; pass_length <= shared_data->password_size;
       pass_length++) {
    max_permutation *= dictionary_length;
    // Enviar a los threads a generarlas
    for (size_t thread_num = 0; thread_num < shared_data->thread_count;
         thread_num++) {
      // Calcular inicio
      private_data[thread_num].start = static_mapping(
          thread_num, max_permutation, shared_data->thread_count);
      // Calcular final
      private_data[thread_num].end = static_mapping(
          thread_num + 1, max_permutation, shared_data->thread_count);
      private_data[thread_num].current_length = pass_length;
      private_data[thread_num].current_permutation = max_permutation;
      private_data[thread_num].zip_archive = zip_open(zip->path, 0, &errorp);
      // Create threads
      pthread_create(&threads[thread_num], NULL, (void*)generate_password,
                     &private_data[thread_num]);
    }

    for (size_t thread_num = 0; thread_num < shared_data->thread_count;
         ++thread_num) {
      pthread_join(threads[thread_num], NULL);
    }
  }
  free(threads);
}

int generate_password(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)private_data->shared_data;
  size_t zip_count = private_data->current_zip;
  zip_info_t* zip = &shared_data->zips->elements[zip_count];
  size_t pass_length = private_data->current_length;
  char* dictionary = shared_data->dictionary;
  size_t dictionary_length = strlen(shared_data->dictionary);

  for (size_t attempts = private_data->start; attempts < private_data->end;
       attempts++) {
    char* new_password = (char*)calloc(pass_length + 1, sizeof(char));
    size_t number = attempts;

    // Construir la cadena new_password
    for (int position = pass_length - 1; position >= 0; position--) {
      new_password[position] = dictionary[number % dictionary_length];
      number /= dictionary_length;
    }
    // new_password[pass_length] = '\0';
    // printf("Thread %zu trying = %s en el zip %zu \n",
    //        private_data->thread_number, new_password, zip_count);
    try_password(zip, new_password, private_data, zip_count);
    free(new_password);

    pthread_mutex_lock(&shared_data->can_open_zip);
    if (&shared_data->zips->elements[zip_count].zip_archive == 0) {
      zip_close(shared_data->zips->elements[zip_count].zip_archive);
    }
    if (shared_data->is_cracked[zip_count]) {
      pthread_mutex_unlock(&shared_data->can_open_zip);
      return PASSWORD_CRACKED;
    }
    pthread_mutex_unlock(&shared_data->can_open_zip);
  }
  return PASSWORD_NOT_CRACKED;
}

int try_password(zip_info_t* zip, char* password, private_data_t* private_data,
                 int64_t zip_number) {
  shared_data_t* shared_data = (shared_data_t*)private_data->shared_data;

  struct zip_stat* file_info = NULL;
  file_info = calloc(256, sizeof(int));
  zip_stat_init(file_info);

  zip_stat_index(private_data->zip_archive, 0, 0, file_info);
  char* txt = calloc(file_info->size + 1, sizeof(char));
  zip_file_t* fd =
      zip_fopen_index_encrypted(private_data->zip_archive, 0, 0, password);
  zip_fread(fd, txt, file_info->size);
  char* result = "CI0117-23a";
  //  encontré la contraseña
  if (strcmp(txt, result) == 0) {
    // guardar la contraseña en el struct
    strncpy(zip->password, password, strlen(password));
    // cerrar el archivo y liberar memoria
    free(txt);
    free(file_info);
    zip_close(private_data->zip_archive);
    pthread_mutex_lock(&shared_data->can_open_zip);
    shared_data->is_cracked[zip_number] = true;
    pthread_mutex_unlock(&shared_data->can_open_zip);
    zip_fclose(fd);
    return PASSWORD_CRACKED;
  }
  free(txt);
  free(file_info);
  return PASSWORD_NOT_CRACKED;
}

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
