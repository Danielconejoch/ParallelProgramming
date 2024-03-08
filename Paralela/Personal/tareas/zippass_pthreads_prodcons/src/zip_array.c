// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#include <assert.h>
#include <zip_array.h>

void zip_array_init(zip_array_t* zip_array) {
  assert(zip_array);
  zip_array->elements = NULL;
  zip_array->capacity = 0;
  zip_array->count = 0;
}

void zip_array_destroy(zip_array_t* zip_array) {
  assert(zip_array);          // Ensure the array pointer is not NULL
  free(zip_array->elements);  // Free the memory allocated for the elements
  zip_array->capacity = 0;    // Reset capacity to 0
  zip_array->count = 0;       // Reset count to 0
}

int64_t array_int_increase_capacity(zip_array_t* zip_array) {
  assert(zip_array);  // Ensure the array pointer is not NULL
  // Calculate new capacity, multiplying by 10
  int64_t new_capacity = 10 * (zip_array->capacity ? zip_array->capacity : 1);
  // Reallocate memory for new capacity
  zip_info_t* new_elements = (zip_info_t*)realloc(
      zip_array->elements, new_capacity * sizeof(zip_info_t));
  if (new_elements) {  // Check if realloc returned a valid pointer
    zip_array->capacity = new_capacity;  // Update capacity
    zip_array->elements = new_elements;  // Update elements pointer
    return EXIT_SUCCESS;                 // Return success
  } else {
    return EXIT_FAILURE;  // Return failure if realloc failed
  }
}

int64_t zip_array_append(zip_array_t* zip_array, zip_info_t* zip_to_add) {
  assert(zip_array);  // Ensure the array pointer is not NULL
  // Check if array is full and tries to increase capacity
  if (zip_array->count == zip_array->capacity) {
    if (array_int_increase_capacity(zip_array) == EXIT_FAILURE) {
      return EXIT_FAILURE;  // Return failure if increasing capacity failed
    }
  }
  // Append the element at the next available position
  zip_array->elements[zip_array->count] = *zip_to_add;
  zip_array->count++;   // Increment the count of elements
  return EXIT_SUCCESS;  // Return success
}
