// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#include "Heat.hpp"

int heatIterations_serial(Data& data, vector<vector<double>>& heat_map) {
  // Stop condition
  bool equilibrium_found = false;

  int k = 0;

  // Declares the new matrix to copy
  vector<vector<double>> new_heat_map(heat_map.size(),
                                      vector<double>(heat_map[0].size(), 0.0));

  // While the equilibrium has not been found
  while (!equilibrium_found) {
    // Assume it has been found to later check just a single odd case where it
    // wasnt found.
    equilibrium_found = true;

    // Transfers heat onto the matrix.
    for (size_t i = 1; i < heat_map.size() - 1; ++i) {
      for (size_t j = 1; j < heat_map[0].size() - 1; ++j) {
        double heat = (new_heat_map[i - 1][j] + new_heat_map[i][j + 1] +
                       new_heat_map[i + 1][j] + new_heat_map[i][j - 1] -
                       (4 * new_heat_map[i][j])) *
                      (data.time * data.thermal_diffusivity) /
                      (data.dimensions * data.dimensions);
        heat_map[i][j] += heat;
      }
    }

    // Checks if any change in the heat of the plate is greater than the
    // sensitivity, if thats the case, the equilibrium wasnt found. Also copies
    // the new matrix in the previous matrix to start a new cycle.
    for (size_t i = 0; i < heat_map.size(); ++i) {
      for (size_t j = 0; j < heat_map[0].size(); ++j) {
        double difference = new_heat_map[i][j] - heat_map[i][j];
        difference = difference < 0 ? -difference : difference;
        if (difference > data.sensitivity) {
          equilibrium_found = false;
        }
        new_heat_map[i][j] = heat_map[i][j];
      }
    }

    // adds one iteration
    k++;
  }

  return k;
}

int begin(int thread_num, int work, int thread_amount) {
  int mod = work % thread_amount;
  int min = (mod) > thread_num ? thread_num : mod;
  return thread_num * (work / thread_amount) + min;
}

int end(int thread_num, int work, int thread_amount) {
  return begin(thread_num + 1, work, thread_amount);
}

int heatIterations(Data& data, vector<vector<double>>& heat_map) {
  // Stop condition
  bool equilibrium_found = false;

  // declares the iterations value
  size_t k = 0;

  // Declares the new matrix to copy
  vector<vector<double>> new_heat_map(heat_map.size(),
                                      vector<double>(heat_map[0].size(), 0.0));

  // Copies the new matrix
  for (size_t i = 0; i < heat_map.size(); ++i) {
    for (size_t j = 0; j < heat_map[0].size(); ++j) {
      new_heat_map[i][j] = heat_map[i][j];
    }
  }
  // Gets the max num threads
  size_t thread_count = data.thread_count;
  if (thread_count > heat_map.size() - 2) {
    thread_count = heat_map.size() - 2;
  }

#pragma omp parallel num_threads(thread_count) default(none)                 \
    shared(new_heat_map, heat_map, data, equilibrium_found, thread_count, k, \
               std::cout)
  {
    int t_num = omp_get_thread_num();
    int t_count = thread_count;
    int work = heat_map.size() - 2;
    int comienzo = begin(t_num, work, t_count) + 1;
    int fin = end(t_num, work, t_count) + 1;

    // While the equilibrium has not been found
    while (!equilibrium_found) {
      #pragma omp barrier
      equilibrium_found = true;

      for (int i = comienzo; i < fin; ++i) {
        for (size_t j = 1; j < heat_map[0].size() - 1; ++j) {
          double heat = (new_heat_map[i - 1][j] + new_heat_map[i][j + 1] +
                         new_heat_map[i + 1][j] + new_heat_map[i][j - 1] -
                         (4 * new_heat_map[i][j])) *
                        (data.time * data.thermal_diffusivity) /
                        (data.dimensions * data.dimensions);
          heat_map[i][j] += heat;
        }
      }
      #pragma omp barrier
      for (int i = comienzo; i < fin; ++i) {
        for (size_t j = 1; j < heat_map[0].size() - 1; ++j) {
          double difference = new_heat_map[i][j] - heat_map[i][j];
          difference = difference < 0 ? -difference : difference;
          if (difference > data.sensitivity) {
            equilibrium_found = false;
          }
          new_heat_map[i][j] = heat_map[i][j];
        }
      }
      // adds one iteration
      #pragma omp single
      k++;
    }
  }

  // saves the iterations in the vector
  return k;
}
