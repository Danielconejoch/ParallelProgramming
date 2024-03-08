// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0

#include <string>
#include "Manager.hpp"

void readArguments(std::vector<zipInfo>& zips) {
  // String used to extract information from getline
  std::string linea;
  std::getline(std::cin, linea);

  // Saves the dictionary
  char* dictionary = new char[linea.length() + 1];
  snprintf(dictionary, linea.length() + 1, "%s", linea.c_str());

  // Saves the value of the password max size
  int64_t passwordSize;
  std::cin >> passwordSize;

  while (std::getline(std::cin, linea)) {
    // Saves path until encounters " \t"
    if (linea.find_first_not_of(" \t") != std::string::npos) {
      // Creates a zipInfo struct
      zipInfo zipInfo;
      // Saves the path in the struct
      zipInfo.zipPath = new char[linea.length() + 1];
      snprintf(zipInfo.zipPath, linea.length() + 1, "%s", linea.c_str());
      // Saves the struct in a vector of structs
      zips.push_back(zipInfo);
    }
  }

  for (auto& zip : zips) {
    // Allocate memory
    zip.password = new char[passwordSize + 1];
    // Init memory
    std::memset(zip.password, 0, passwordSize + 1);
    // boolean value that indicates if the password was found
    zip.passwordFound = false;
    // calls the method bruteforce for that zip
  }

  brute_force(zips, dictionary, passwordSize);

  // Prints the zip with the password (if the password was cracked)
  for (auto& zip : zips) {
    if (zip.password[0] != '\0') {
      // zip and password
      std::cout << zip.zipPath << " " << zip.password << std::endl;
    } else {
      // only zip because password was not found
      std::cout << zip.zipPath << std::endl;
    }
  }

  // Free memory
  delete[] dictionary;
  for (auto& zip : zips) {
    delete[] zip.zipPath;
    delete[] zip.password;
  }
}
