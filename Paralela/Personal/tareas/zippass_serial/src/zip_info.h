// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#ifndef ZIP_INFO_H
#define ZIP_INFO_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>

/**
 *
 * @brief ahorita lo agregoxd
 */
typedef struct {
  char* password;
  char* path;
  zip_t* zip_archive;
} zip_info_t;

/**
 * @brief Initializes a zip_info struct
 * @param info Pointer to the zip_info struct
 * @param new_path Path to set for zip_info->path
 * @param pass_size Max length of the password
 */
void zip_info_init(zip_info_t* zip_info, char* new_path, int64_t pass_size);

/**
 * @brief Prints information about the zip_info struct
 * @param info The zip_info struct to print it's information
 */
void zip_info_destroy(zip_info_t* zip_to_destroy);

/**
 * @brief Frees memory allocated for a zip_info struct
 * @param info Pointer to the zip_info struct
 */
void print_zip_info(zip_info_t* zip_to_print);

#endif
