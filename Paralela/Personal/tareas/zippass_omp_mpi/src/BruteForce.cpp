// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#include "BruteForce.hpp"

void brute_force(std::vector<zipInfo>& zips, char* dictio, int64_t pass_size) {
  // Dictionary size
  int64_t length = strlen(dictio);
  // Initialize the variable max permutations
  int64_t max_permutation = 1;
  // Initialize the variable number
  int64_t number = 0;
  // vector to save passwords
  std::vector<char*> passwords;

  // Start generating passwords
  for (int64_t pass_length = 1; pass_length <= pass_size; pass_length++) {
    max_permutation *= length;
    // Loop through all possible permutations
    for (int64_t attempts = 0; attempts < max_permutation; attempts++) {
      char* new_password = new char[pass_length + 1];
      number = attempts;

      // Build the 'new_password'
      for (int position = pass_length - 1; position >= 0; position--) {
        new_password[position] = dictio[number % length];
        number /= length;
      }
      // Add '\0' at the end of the password
      new_password[pass_length] = '\0';
      // Saves the password in the vector
      passwords.push_back(new_password);
    }
  }
  // Tries all passwords
  for (auto& zip : zips) {
    try_password(&zip, passwords);
  }

  // Free allocated memory
  for (auto& password : passwords) {
    delete[] password;
  }

}

void try_password(zipInfo* zip, std::vector<char*>& passwords) {
  // Creates a vector of zip_t*
  std::vector<zip_t*> zipOpens;
  // Get max threads num
  size_t num_threads = omp_get_max_threads();
  // Opens all zips
  int error = 0;
  for (size_t i = 0; i < num_threads; i++) {
    zipOpens.push_back(zip_open(zip->zipPath, 0, &error));
  }

  // Stop Condition
  bool passwordFound = false;
  #pragma omp parallel for default(none) num_threads(num_threads) \
  shared(passwordFound, zipOpens, zip, passwords) schedule(dynamic)
  for (size_t i = 0; i < passwords.size(); i++) {
    // Tries the password if it wasn't found yet
    if (!passwordFound) {
      // Tries to open the zip file
      size_t thread_num = omp_get_thread_num();
      zip_file_t* fd =
      zip_fopen_index_encrypted(zipOpens[thread_num], 0, 0, passwords[i]);
      // If the zip was opened
      if (fd != NULL) {
        zip_stat_t* file_info = new zip_stat_t;
        zip_stat_init(file_info);

        zip_stat_index(zipOpens[thread_num], 0, 0, file_info);
        // txt to read what's inside the zip
        char* txt = new char[file_info->size + 1];

        // Tries to read the content inside the zip
        zip_fread(fd, txt, file_info->size);

        // Add '\0' at the end of the txt
        txt[file_info->size] = '\0';

        // This char it's what's inside the zips
        const char* result = "CI0117-23a";

        // Close the file
        zip_fclose(fd);
        // Found password
        if ((strcmp(txt, result) == 0)) {
          #pragma omp critical(can_save_password)
          {
            // Save the password in the struct
            strncpy(zip->password, passwords[i], strlen(passwords[i]));
            // Stop condition
            passwordFound = true;
          }
          // Free memory
          delete[] txt;
          delete file_info;
        } else {
          // Password not found
          delete[] txt;
          delete file_info;
        }
      }
    }
  }
  // Close all zips
  for (size_t i = 0; i < num_threads; i++) {
    zip_close(zipOpens[i]);
  }
}
