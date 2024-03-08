// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#include "zip_info.h"

void zip_info_init(zip_info_t* zip_info, char* new_path, int64_t pass_size) {
  // Initialize the zip_info structure with the new path and a maximum password
  // size
  zip_info->path = (char*)calloc(strlen(new_path) + 1, sizeof(char));
  strncpy(zip_info->path, new_path, strlen(new_path));
  zip_info->password = (char*)calloc(pass_size + 1, sizeof(char));
  zip_info->zip_archive = NULL;
}

void zip_info_destroy(zip_info_t* zip_to_destroy) {
  // Free the memory allocated for the path and password
  free(zip_to_destroy->path);
  free(zip_to_destroy->password);
}

void print_zip_info(zip_info_t* zip_to_print) {
  if (strlen(zip_to_print->password) != 0) {
    // Print information about the path and, if present, the password
    printf("%s %s\n", zip_to_print->path, zip_to_print->password);
  } else {
    // Print only the path if there is no password
    printf("%s\n", zip_to_print->path);
  }
}
