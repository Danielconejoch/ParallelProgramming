// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#ifndef ZIP_ARRAY_H
#define ZIP_ARRAY_H

#include <stdio.h>
#include <zip_info.h>

typedef struct {
  zip_info_t* elements;
  size_t capacity;
  size_t count;
} zip_array_t;

/**
 * @brief Initializes an array of zip_infos
 *
 * @param zip_array Pointer to the array to initialize
 */
void zip_array_init(zip_array_t* zip_array);

/**
 * @brief Frees memory allocated for a zip_array struct
 *
 * @param zip_array Pointer to the array to destroy
 */
void zip_array_destroy(zip_array_t* zip_array);

/**
 * @brief Increases the capacity of the array allocating memory
 *
 * @param zip_array Pointer to the array to increase capacity
 */
int64_t array_int_increase_capacity(zip_array_t* zip_array);

/**
 * @brief Appends an zip_info struct to the zip_array
 *
 * @param zip_array Pointer to the array
 * @param element The zip_info to append
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int64_t zip_array_append(zip_array_t* zip_array, zip_info_t* zip_to_add);

#endif
