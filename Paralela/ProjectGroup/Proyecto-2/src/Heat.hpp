// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#ifndef HEAT_HPP
#define HEAT_HPP

#include <omp.h>

#include <string>
#include <vector>

#include "Ordenador.hpp"

using std::vector;

/**
 * @brief Performs heat iterations on a heat map using a serial approach.
 *
 * This function calculates the heat distribution on a 2D heat map by
 * iteratively transferring heat from neighboring cells. It continues the
 * iterations until the equilibrium is reached, where the change in heat of the
 * plate is below a specified sensitivity threshold.
 *
 * @param data The data structure containing the simulation parameters.
 * @param heat_map The 2D vector representing the heat map.
 * @return The number of iterations performed until equilibrium is reached.
 */
int heatIterations_serial(Data& data, vector<vector<double>>& heat_map);

/**
 * Calculates the work assignment to a thread in a multi-threaded environment.
 *
 * @param thread_num The identification number of the current thread.
 * @param work The total amount of work to be done.
 * @param thread_amount The total number of available threads.
 * @return The work assignment for the current thread.
 */
int begin(int thread_num, int work, int thread_amount);

/**
 * Calculates the end of work assignment for a thread in a multi-threaded
 * environment.
 *
 * @param thread_num The identification number of the current thread.
 * @param work The total amount of work to be done.
 * @param thread_amount The total number of available threads.
 * @return The end of work assignment for the current thread.
 */
int end(int thread_num, int work, int thread_amount);

/**
 * Perform iterations to simulate heat diffusion across a 2D matrix.
 *
 * @param data The data structure containing simulation parameters.
 * @param heat_map The 2D matrix representing the heat distribution.
 */
int heatIterations(Data& data, vector<vector<double>>& heat_map);

#endif  // HEAT_HPP
