// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#ifndef PASSWORD_CRACKER_H
#define PASSWORD_CRACKER_H

#include <zip_array.h>

/**
 * @brief Method that opens a zip and uses the password to check if we can open
 * the file, and saves the password on the struct if it's correct
 *
 * @param zip zip_info struct
 * @param password password to try
 *
 * @return error 1 or 0
 */
int try_password(zip_info_t* zip, char* password);

/**
 *
 *
 *
 *
 *
 */

void brute_force(zip_info_t* zip_info, char* dictionary, int64_t pass_size);

// void pruebas(char* dictionary, int64_t pass_size);
#endif
