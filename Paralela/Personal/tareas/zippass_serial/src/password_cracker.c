// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#include <password_cracker.h>
#define PASSWORD_CRACKED 1
#define PASSWORD_NOT_CRACKED 0

void brute_force(zip_info_t* zip, char* dictionary, int64_t pass_size) {
  int64_t dictionary_length = strlen(dictionary);
  int64_t max_permutation = 1;
  int64_t number = 0;

  for (int64_t pass_length = 1; pass_length <= pass_size; pass_length++) {
    max_permutation *= dictionary_length;

    for (int64_t attempts = 0; attempts < max_permutation; attempts++) {
      char* new_password = (char*)calloc(pass_length + 1, sizeof(char));
      number = attempts;

      // Construir la cadena new_password
      for (int position = pass_length - 1; position >= 0; position--) {
        new_password[position] = dictionary[number % dictionary_length];
        number /= dictionary_length;
      }
      new_password[pass_length] = '\0';
      // printf("%s\n", new_password);
      if (try_password(zip, new_password) == PASSWORD_CRACKED) {
        free(new_password);
        return;
      }
      free(new_password);
    }
  }
}

int try_password(zip_info_t* zip, char* password) {
  if (!fopen(zip->path, "r")) return -2;

  int errorp = 0;

  zip->zip_archive = zip_open(zip->path, 0, &errorp);
  struct zip_stat* file_info = NULL;
  file_info = calloc(256, sizeof(int));
  zip_stat_init(file_info);

  zip_stat_index(zip->zip_archive, 0, 0, file_info);

  char* txt = calloc(file_info->size + 1, sizeof(char));

  zip_file_t* fd = zip_fopen_index_encrypted(zip->zip_archive, 0, 0, password);
  zip_fread(fd, txt, file_info->size);
  char* result = "CI0117-23a";
  // encontré la contraseña
  if (strcmp(txt, result) == 0) {
    // guardar la contraseña en el struct
    strncpy(zip->password, password, strlen(password));
    // cerrar el archivo y liberar memoria
    free(txt);
    free(file_info);
    zip_close(zip->zip_archive);
    zip_fclose(fd);
    return PASSWORD_CRACKED;
  }
  free(txt);
  free(file_info);
  zip_close(zip->zip_archive);

  return PASSWORD_NOT_CRACKED;
}
