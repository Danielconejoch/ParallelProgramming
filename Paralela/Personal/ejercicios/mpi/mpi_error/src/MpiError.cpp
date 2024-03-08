#include "MpiError.hpp"
#include "Mpi.hpp"

#include <string>

MpiError::MpiError(const std::string &message, const Mpi &mpi) : std::runtime_error (
  mpi.getProcessHostname() + ":"
  + std::to_string(mpi.getProcessNumber()) + ":" + message
  ) {};

MpiError::MpiError(const std::string &message, const Mpi &mpi
, const int threadNumer)
  : std::runtime_error(mpi.getProcessHostname() + ":"
    + std::to_string(mpi.getProcessNumber()) + ":"
    + std::to_string(threadNumer) + ": " + message
    ) {};
