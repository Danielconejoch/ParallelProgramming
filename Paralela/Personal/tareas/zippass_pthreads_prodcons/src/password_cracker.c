// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0

#include "password_cracker.h"

#include "simulation.h"

#define PASSWORD_CRACKED 1
#define PASSWORD_NOT_CRACKED 0

/**
 * @brief Method that opens a zip and uses the password to check if we can open
 * the file, and saves the password on the struct if it's correct
 *
 * @param zip zip_info struct
 * @param password password to try
 *
 * @return error 1 or 0
 */
int try_password(zip_info_t* zip, char* password, private_data_t* private_data,
                 int64_t zip_number);

void brute_force(void* data) {
  shared_data_t* shared_data = (shared_data_t*)data;

  int64_t dictionary_length = strlen(shared_data->dictionary);
  int64_t max_permutation = 1;
  int64_t number = 0;

  // largo de la contrasena de momento
  for (int64_t pass_length = 1; pass_length <= shared_data->password_size;
       pass_length++) {
    max_permutation *= dictionary_length;

    // attemps hasta que se alcance el maximo posible basado en las
    // permutaciones
    for (int64_t attempts = 0; attempts < max_permutation; attempts++) {
      char* new_password = (char*)calloc(pass_length + 1, sizeof(char));
      number = attempts;

      // Construir la new new_password
      for (int position = pass_length - 1; position >= 0; position--) {
        new_password[position] =
            shared_data->dictionary[number % dictionary_length];
        number /= dictionary_length;
      }
      // new_password[pass_length] = '\0';
      // printf("%s\n", new_password);
      // enqueue la contrasena
      queue_enqueue(shared_data->password_queue, new_password);
      if (shared_data->all_cracked) {
        // free(new_password);
        return;
      }
    }
  }
  // enqueue condicion de parada
  for (int64_t i = 0; i < shared_data->thread_count; i++) {
    char* stop_condition = "-1";
    queue_enqueue(shared_data->password_queue, stop_condition);
  }
}

void consume_password(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = (shared_data_t*)private_data->shared_data;
  while (true) {
    // sigue hasta que todas las contrasenas se encontraron o hasta que se
    // prueben todas
    if (shared_data->all_cracked) {
      break;
    }
    // dequeue de la contrasena
    char* password_to_try = queue_dequeue(shared_data->password_queue);
    if (strcmp(password_to_try, "-1") == 0) {
      // printf("Thread %" PRIu64 " finished\n", private_data->thread_number);
      break;
    }
    // ignora un zip, si la contrasena ya se encontro
    for (uint64_t i = 0; i < shared_data->zips->count; i++) {
      pthread_mutex_lock(shared_data->can_open_zip);
      if (shared_data->is_cracked[i] == true) {
        pthread_mutex_unlock(shared_data->can_open_zip);
        continue;
      }
      pthread_mutex_unlock(shared_data->can_open_zip);
      // printf("Thread %" PRIu64 " trying password: %s on zip: %s\n",
      //        private_data->thread_number, password_to_try,
      //        (shared_data->zips->elements[i].path));
      try_password(&shared_data->zips->elements[i], password_to_try,
                   private_data, i);
    }
    // free password
    free(password_to_try);
  }
}

int try_password(zip_info_t* zip, char* password, private_data_t* private_data,
                 int64_t zip_number) {
  shared_data_t* shared_data = (shared_data_t*)private_data->shared_data;

  int errorp = 0;
  // abre el zip
  pthread_mutex_lock(shared_data->can_open_zip);
  private_data->zip_archive = zip_open(zip->path, 0, &errorp);
  pthread_mutex_unlock(shared_data->can_open_zip);

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
    // cierra el zip si ya encontro la contrasena
    zip_close(private_data->zip_archive);
    pthread_mutex_lock(shared_data->can_open_zip);
    // marca ese zip como cracekd
    shared_data->is_cracked[zip_number] = true;
    // suma al cracked count
    shared_data->cracked_count++;
    pthread_mutex_unlock(shared_data->can_open_zip);
    zip_fclose(fd);
    // cambia a true si la cantidad de zips cracked es igual a la cantidad de
    // zips existentes (condicion de parada)
    if (shared_data->cracked_count == shared_data->zips->count) {
      shared_data->all_cracked = true;
    }
    return PASSWORD_CRACKED;
  }
  // libera memoria
  free(txt);
  free(file_info);
  pthread_mutex_lock(shared_data->can_open_zip);
  // cierra el zip si no se encontro la contrasena
  zip_close(private_data->zip_archive);
  pthread_mutex_unlock(shared_data->can_open_zip);
  return PASSWORD_NOT_CRACKED;
}
