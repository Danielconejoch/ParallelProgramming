#ifndef MPI_H
#define MPI_H

#include <string>
#include <mpi.h>
#include <stdexcept>

class Mpi {
  private:
  int process_number;
  int process_count;
  std::string process_hostname;

  public:
  Mpi(int argc, char* argv[]) {
    if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
      this->process_number = -1;
      if (MPI_Comm_rank(MPI_COMM_WORLD, &this->process_number) != MPI_SUCCESS) {
        throw std::runtime_error("Error: couldn't get rank");
      }
      this->process_count = -1;
      if (MPI_Comm_size(MPI_COMM_WORLD, &this->process_count) != MPI_SUCCESS) {
        throw std::runtime_error("Error: couldn't get size");
      }
      char hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
      int hostname_length = -1;
      if (MPI_Get_processor_name(hostname, &hostname_length) != MPI_SUCCESS) {
        throw std::runtime_error("Error: could't get processor_name");
      }
      this->process_hostname = std::string(hostname, hostname_length);
    } else {
      throw std::runtime_error("Error: couldn't init");
    }
  }

  ~Mpi() {
    MPI_Finalize();
  }

  inline int getProcessNumber() const {
    return this->process_number;
  }
  inline int getProcessCount() const {
    return this->process_count;
  }
  inline std::string getProcessHostname() const {
    return this->process_hostname;
  }
  int rank() {
    return this->process_number;
  }
  int size() {
    return this->process_count;
  }
};


#endif