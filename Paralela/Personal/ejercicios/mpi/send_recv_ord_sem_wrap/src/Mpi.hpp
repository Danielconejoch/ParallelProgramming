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

  static inline MPI_Datatype map(bool) { return MPI_C_BOOL; }
  static inline MPI_Datatype map(char) { return MPI_CHAR; }
  static inline MPI_Datatype map(unsigned char) { return MPI_UNSIGNED_CHAR; }
  static inline MPI_Datatype map(short) { return MPI_SHORT; }
  static inline MPI_Datatype map(unsigned short) { return MPI_UNSIGNED_SHORT; }
  static inline MPI_Datatype map(int) { return MPI_INT; }
  static inline MPI_Datatype map(unsigned) { return MPI_UNSIGNED; }
  static inline MPI_Datatype map(long) { return MPI_LONG; }
  static inline MPI_Datatype map(unsigned long) { return MPI_UNSIGNED_LONG; }
  static inline MPI_Datatype map(long long) { return MPI_LONG_LONG; }
  static inline MPI_Datatype map(unsigned long long) { return MPI_UNSIGNED_LONG_LONG; }
  static inline MPI_Datatype map(float) { return MPI_FLOAT; }
  static inline MPI_Datatype map(double) { return MPI_DOUBLE; }
  static inline MPI_Datatype map(long double) { return MPI_LONG_DOUBLE; }

  template<typename DataType>
  int send(DataType value, int toProcess, int tag=0) {
    return MPI_Send(&value, /*count*/ 1, map(value), toProcess, /*tag*/ tag
        , MPI_COMM_WORLD);
  }

  template<typename DataType>
  int receive(DataType value, int fromProcess, int tag=MPI_ANY_TAG) {
    return MPI_Recv(&value, /*count*/ 1, map(value)
    , fromProcess, /*tag*/ tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
};


#endif