#include <omp.h>
#include <iostream>
#include <vector>
#include <cmath>

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  // Read numbers 
  std::vector<double> numbers;
  double number = 0.0;
  std::cout << "Type the numbers separated by spaces and CTRL+D to finish: ";

  // Read the numbers from the string
  while (std::cin >> number) {
    numbers.push_back(number);
  }

  // Print the array in its original order
  std::cout << "Original array: ";
  for (const auto& num : numbers) {
    std::cout << num << " ";
  }
  std::cout << std::endl;

  // Sort the array concurrently
  #pragma omp parallel for num_threads(thread_count) default(none) shared(numbers)
  for (size_t i = 0; i < numbers.size(); ++i) {
    for (size_t j = i + 1; j < numbers.size(); ++j) {
      if (numbers[i] > numbers[j]) {
        std::swap(numbers[i], numbers[j]);
      }
    }
  }

  // Print the array after sorting
  std::cout << "Sorted array: ";
  for (const auto& num : numbers) {
    std::cout << num << " ";
  }
  std::cout << std::endl;


  // Stores the sum
  double sum = 0.0;

  // Stores the minimum value
  double minimun = numbers[0];

  // Stores the maximum value
  double maximun = numbers[0];

  // Stores sum squared
  double deviation = 0.0;

  // Calculate median
  double median = 0.0;
  if (numbers.size() % 2 == 0) {
    median = (numbers[numbers.size() / 2 - 1] + numbers[numbers.size() / 2]) / 2.0;
  } else {
    median = numbers[numbers.size() / 2];
  }

  #pragma omp parallel for num_threads(thread_count) default(none) shared(numbers) \
    reduction(+:sum) reduction(min:minimun) reduction(max:maximun) reduction(+:deviation)
  for (size_t i = 0; i < numbers.size(); ++i) {
    // Calculates the average
    sum += numbers[i];

    // Starts to calculate deviation
    deviation += numbers[i] * numbers[i];

    // Stores the minimum value
    if (numbers[i] < minimun) {
      minimun = numbers[i];
    }

    // Stores the maximum value
    if (numbers[i] > maximun) {
      maximun = numbers[i];
    }
  }

  // Prints min value
  std::cout << "Min: " << minimun << std::endl;

  // Prints max value
  std::cout << "Max: " << maximun << std::endl;

  // Prints median
  std::cout << "Median: " << median << std::endl;
  
  // Prints average
  const double average = sum / numbers.size();
  std::cout << "Average: " << average << std::endl;

  // Prints variance
  const double variance = (deviation - (average * average)) / numbers.size();
  std::cout << "Variance: " << variance << std::endl;

  // Prints Standard deviation
  deviation = std::sqrt(variance);
  std::cout << "Deviation: " << deviation << std::endl;

  return 0;
}
