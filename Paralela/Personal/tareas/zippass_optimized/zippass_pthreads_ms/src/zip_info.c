// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#include "zip_info.h"

void zip_info_init(zip_info_t* zip_info, char* new_path, int64_t pass_size) {
  zip_info->path = (char*)calloc(strlen(new_path) + 1, sizeof(char));
  strncpy(zip_info->path, new_path, strlen(new_path));
  zip_info->password = (char*)calloc(pass_size + 1, sizeof(char));
  zip_info->zip_archive = NULL;
}

void zip_info_destroy(zip_info_t* zip_to_destroy) {
  free(zip_to_destroy->path);
  free(zip_to_destroy->password);
}

void print_zip_info(zip_info_t* zip_to_print) {
  printf("%s %s\n", zip_to_print->path, zip_to_print->password);
}
