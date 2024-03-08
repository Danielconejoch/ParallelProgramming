// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <chrono>
#include "Manager.hpp"

int main() {
  // Starts time
  auto start = std::chrono::high_resolution_clock::now();

  std::vector<zipInfo> zipInfos;
  readArguments(zipInfos);

  // Stops timpe
  auto stop = std::chrono::high_resolution_clock::now();

  // Calculates time
  auto duration =
  std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  double seconds = duration.count() / 1000.0;
  std::cout << "Tiempo de ejecución: " << seconds << " segundos" << std::endl;
  return EXIT_SUCCESS;
}
