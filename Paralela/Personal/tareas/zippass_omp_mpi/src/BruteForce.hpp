// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0

#include <cstdlib>
#include <cstring>
#include <vector>
#include <omp.h>
#include <zip.h>
#pragma once

#include "Manager.hpp"

struct zipInfo;
/**
 * @brief Attempt to crack the password of a ZIP file using a bruteforce
 * @param zip Pointer to a struct containing ZIP file information.
 * @param dictionary string that contains the password dictionary.
 * @param pass_size Maximum size of the password to generate.
 */
void brute_force(std::vector<zipInfo>& zips, char* dictio, int64_t pass_size);

/**
 * @brief Method that opens a zip and uses the password to check if we can open
 * the file, and saves the password on the struct if it's correct
 *
 * @param zip zipInfo struct
 * @param password password to try
 * @param passFound stop condition
 *
 */
void try_password(zipInfo* zip, std::vector<char*>& passwords);
