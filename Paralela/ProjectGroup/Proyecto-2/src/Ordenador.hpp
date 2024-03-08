// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#ifndef ORDENADOR_HPP
#define ORDENADOR_HPP

#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using std::vector;

#include "Mpi.hpp"
/**
 * @brief Represents data related to a simulation or experiment.
 *
 * This struct encapsulates various parameters and information related to a
 * simulation or experiment.
 */
struct Data {
  // name of the bin file
  std::string bin_file;
  // Time parameter for the heat simulation.
  double time;
  // Thermal diffusivity value of the material
  double thermal_diffusivity;
  // Dimensions of evert cell of the plate
  double dimensions;
  // sensitibity or epsilon equilibrium
  double sensitivity;
  // Path to the binary file.
  std::string bin_path;
  // Max threads or threads that the user wanted
  size_t thread_count;
  // iterations until equilibrium is reached
  size_t iterations;
  // saves the time in the format requires for the code
  std::string format_time;
  //
  vector<vector<double>> matrix;
};

/**
 * @class Ordenador
 * @brief Class representing an Ordenador object.
 *
 * The Ordenador class is responsible for organizing and processing data.
 * It provides methods for reading binary files and performing operations on the
 * data.
 */
class Ordenador {
 public:
  /**
   * @brief Default constructor for Ordenador.
   */
  Ordenador() {}
  /**
   * @brief Read binary file and store plate data in heat_map.
   * @param bin_path Path to the binary file.
   * @return True if read successful, false otherwise.
   */
  bool read_bin(Data& data);
};

/**
 * @brief Extract the directory portion of a file path.
 * @param directory Full file path.
 * @return Directory portion of the file path.
 */
std::string getPlateDirectory(std::string directory);

/**
 * @brief Serially processes a vector of Data objects.
 *
 * This function iterates over each Data object in the vector and performs
 * heatIterations_serial on its matrix. It then updates the iterations and
 * format_time properties of the Data object based on the result of the
 * heatIterations_serial function.
 *
 * @param datas The vector of Data objects to be processed.
 */
void serial(vector<Data>& datas);

/**
 * @brief Receives data from multiple processes and updates the datas vector.
 *
 * This function receives data from multiple processes using MPI_Recv and
 * updates the datas vector accordingly. It receives the rank, height, width,
 * data_num, matrix, and k values from each process and stores them in the datas
 * vector. The received matrix values are stored in the datas[data_num].matrix.
 * The number of iterations and the formatted time are also updated in the datas
 * vector.
 *
 * @param datas The vector of Data objects to be updated.
 */
void first_process(vector<Data>& datas);

/**
 * @brief Perform other process on the data using MPI communication.
 *
 * This function performs a series of computations on the data using MPI
 * communication. It divides the data among the MPI processes and sends the
 * necessary information to the root process.
 *
 * @param mpi The MPI object for communication.
 * @param datas The vector of Data objects to be processed.
 */
void other_process(Mpi& mpi, vector<Data>& datas);

/**
 * @brief Main function to start the program.
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int start(int argc, char* argv[]);

/**
 * Reads simulation information from a string, populates a Data object, and
 * initiates data processing.
 *
 * @param data Reference to a Data object where the parsed information will be
 * stored.
 * @param simulation_info The string containing simulation information to be
 * processed.
 * @param directory The directory path containing the simulation files.
 * @param thread_count The number of threads used for processing.
 */
void readArguments(Data& data, const std::string& simulation_info,
                   std::string directory, size_t thread_count);

/**
 * @brief Calculates the starting index for a given thread in a parallel
 * computation.
 *
 * This function calculates the starting index for a specific thread in a
 * parallel computation, based on the total number of threads, the total amount
 * of work, and the current thread number.
 *
 * @param thread_num The number of the current thread.
 * @param work The total amount of work to be divided among the threads.
 * @param thread_amount The total number of threads.
 * @return The starting index for the given thread.
 */
int begin2(int thread_num, int work, int thread_amount);

/**
 * @brief Calls the begin2 function with an incremented thread number.
 *
 * This function is used to continue the execution of the program by calling the
 * begin2 function with an incremented thread number. It takes the current
 * thread number, the amount of work to be done, and the total number of threads
 * as parameters.
 *
 * @param thread_num The current thread number.
 * @param work The amount of work to be done.
 * @param thread_amount The total number of threads.
 * @return The result of the begin2 function with an incremented thread number.
 */
int end2(int thread_num, int work, int thread_amount);

/**
 * Writes a report containing data into a tab-separated values (TSV) file.
 *
 * @param data A vector containing Data objects to be included in the report.
 * @param file The file name along with its path where the report will be saved.
 * @param directory The directory path where the report will be stored.
 */
void writeReport(std::vector<Data>& data, std::string file,
                 std::string directory);

/**
 * Formats the given time in seconds into a string representation (YYYY/MM/DD
 * hh:mm:ss).
 *
 * @param seconds The time in seconds to be formatted.
 * @return A string representing the formatted time (YYYY/MM/DD hh:mm:ss).
 */
std::string format_time(const time_t seconds);
#endif  // ORDENADOR_HPP
