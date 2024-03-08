// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0

#include <iostream>
#include <vector>
#pragma once

#include "BruteForce.hpp"

/**
 * @struct zipInfo
 * @brief Structure to hold information related to a ZIP file.
 */
struct zipInfo {
    // the path to the zip
    char* zipPath;
    // password (empty at the beginning but if is found it will change to that)
    char* password;
    // if true means that the password was found or that there aren't more
    // passwords to try, false otherwise
    bool passwordFound;
};

/**
 * @brief Reads and processes arguments from a txt file.
 *
 * Reads a file containing information about ZIP archives and their passwords.
 *
 * @param filename The name of the file to read.
 * @param zipInfos A vector to store zipInfo structs containing archive information.
 */
void readArguments(std::vector<zipInfo>& zipInfos);

