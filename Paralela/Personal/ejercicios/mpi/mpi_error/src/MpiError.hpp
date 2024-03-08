#ifndef MPIERROR_H
#define MPIERROR_H

#include <stdexcept>

class Mpi;

class MpiError : public std::runtime_error {
  public:
  explicit MpiError(const std::string &message) : std::runtime_error(message) {};
  MpiError(const std::string &message, const Mpi &mpi);
  MpiError(const std::string &message, const Mpi &mpi, const int threadNumber);
};

#endif