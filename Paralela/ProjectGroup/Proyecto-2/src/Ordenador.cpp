// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#include "Ordenador.hpp"

#include "Heat.hpp"

namespace fs = std::filesystem;

std::string getPlateDirectory(std::string directory) {
  // Find the position of the last (/)
  size_t lastSeparatorPos = directory.rfind('/');

  // Returns the path without 'job###.txt'
  return directory.substr(0, lastSeparatorPos + 1);
}

void serial(vector<Data>& datas) {
  // For cycle that iterates over the vector of Data
  for (size_t i = 0; i < datas.size(); ++i) {
    // Calls heatIterations_serial
    int k = heatIterations_serial(datas[i], datas[i].matrix);
    // Saves the iterations and the time in the format required
    datas[i].iterations = k;
    datas[i].format_time = format_time(k * datas[i].time);
  }
}

void first_process(vector<Data>& datas) {
  // Cycle that iterates over the vector of Data
  for (size_t i = 0; i < datas.size(); ++i) {
    // Declaration of variables
    int rank = 0;
    int height = 0;
    int width = 0;
    int k = 0;
    int data_num = 0;
    // Receives the data from the other processes
    MPI_Recv(&rank, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Recv(&height, 1, MPI_INT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&width, 1, MPI_INT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&data_num, 1, MPI_INT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // Creates a vector to store the matrix
    vector<double> v(height * width, 0.0);
    // Receives the matrix
    MPI_Recv(v.data(), height * width, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    MPI_Recv(&k, 1, MPI_INT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // Stores the matrix in the datas vector
    for (int j = 0; j < height; ++j) {
      for (int k = 0; k < width; ++k) {
        datas[data_num].matrix[j][k] = v[k + j * width];
      }
    }
    // Saves the iterations and the time in the format required
    datas[data_num].iterations = k;
    datas[data_num].format_time = format_time(k * datas[data_num].time);
  }
}

void other_process(Mpi& mpi, vector<Data>& datas) {
  // Declaration of start and end
  int inicio = begin2(mpi.rank() - 1, datas.size(), mpi.size() - 1);
  int fin = end2(mpi.rank() - 1, datas.size(), mpi.size() - 1);
  for (int i = inicio; i < fin; ++i) {
    // Update k value
    int k = heatIterations(datas[i], datas[i].matrix);
    // Declaration of the vector v
    vector<double> v;
    // Declaration of variables
    int rank = mpi.rank();
    int height = datas[i].matrix.size();
    int width = datas[i].matrix[0].size();
    // Stores the matrix data in the vector v
    for (int j = 0; j < height; ++j) {
      for (int k = 0; k < width; ++k) {
        v.push_back(datas[i].matrix[j][k]);
      }
    }
    // Sends the data to the root process
    MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&height, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&width, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    int data_num = i;
    MPI_Send(&data_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    MPI_Send(v.data(), height * width, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    MPI_Send(&k, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
}

int start(int argc, char* argv[]) {
  // Checks if the user added the file when executing the program
  if (argc < 2) {
    std::cerr << "Entrada de datos: " << argv[0]
              << "tests/<nombre del archivo>"
                 " (cantidad de hilos) <- (opcional) "
              << "(tests/<nombre carpeta salida de datos>/) <- (opcional)"
              << std::endl;
    return EXIT_FAILURE;
  }

  // Receives the job_file to work with
  std::string job_txt = argv[1];

  // Tries to open de job_txt
  std::ifstream archivo(job_txt);

  // Checks if it was able to open
  if (!archivo.is_open()) {
    std::cerr << "No se pudo abrir el archivo " << job_txt
              << ".\nRevisa el path ingresado e intentalo de nuevo."
              << std::endl;
    // Ends the execution if it was not able to open de archive
    return EXIT_FAILURE;
  }

  // Read the thread count if the user adds it, in other case we assume the
  // amount of threads available in the system
  size_t thread_count =
      (argc >= 3) ? std::atoi(argv[2]) : omp_get_max_threads();

  // Gets the output directory (if provided otherwise use tests/output
  std::string output_dir = "tests/report";
  if (argc >= 4) {
    output_dir = argv[3];
  }

  if (!fs::exists(output_dir)) {
    fs::create_directory(output_dir);
  }

  // A string to store every line of the txt
  std::string read_line;

  // Creates an instance for the struct Data
  std::vector<Data> datas;
  size_t counter = 0;

  while (std::getline(archivo, read_line)) {
    // Calls read arguments
    datas.resize(counter + 1);
    readArguments(datas[counter], read_line, job_txt, thread_count);
    datas.push_back(datas[counter]);
    counter++;
  }

  Mpi mpi(argc, argv);

  if (mpi.size() <= 2) {
    if (mpi.rank() == 0) {
      serial(datas);
      writeReport(datas, job_txt, output_dir);
    }
  } else {
    if (mpi.rank() == 0) {
      first_process(datas);
      writeReport(datas, job_txt, output_dir);
    } else {
      other_process(mpi, datas);
    }
  }

  return EXIT_SUCCESS;
}

void readArguments(Data& data, const std::string& simulation_info,
                   std::string directory, size_t thread_count) {
  // Process every line and store it into the struct data
  std::istringstream iss(simulation_info);

  if (iss >> data.bin_file) {
    // We call getPlateDirectory to update the bin_file path using the
    // path of the directory where the plate is and the plate name given in
    // the job###.txt
    data.bin_path = getPlateDirectory(directory) + data.bin_file;

    // Saves the other values received in the job.txt
    iss >> data.time >> data.thermal_diffusivity >> data.dimensions >>
        data.sensitivity;

    data.thread_count = thread_count;
    // Create an instance of Ordenador
    Ordenador ord = Ordenador();
    // Call read_bin to obtain the information on the plate and store it
    // in a matrix
    ord.read_bin(data);
  } else {
    std::cerr << "Error al procesar los datos: " << simulation_info
              << std::endl;
  }
}

bool Ordenador::read_bin(Data& data) {
  std::ifstream file;
  uint64_t plate_size[2];
  const std::string plate_filename = data.bin_path;
  try {
    file.open(plate_filename, std::ios::binary | std::ios::out);

    if (!file.read(reinterpret_cast<char*>(&plate_size), sizeof(plate_size))) {
      std::cerr << "Error: Could not read file." << std::endl;
    }

    uint64_t height = plate_size[0];
    uint64_t width = plate_size[1];

    double plate[height][width];
    if (!file.read(reinterpret_cast<char*>(&plate), sizeof(plate))) {
      std::cerr << "Error: Could not read file." << std::endl;
    }

    file.close();

    data.matrix = vector<vector<double>>(height, vector<double>(width, 0.0));
    for (uint64_t i = 0; i < height; ++i) {
      for (uint64_t j = 0; j < width; ++j) {
        data.matrix[i][j] = plate[i][j];
      }
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return true;
}

void writeReport(std::vector<Data>& data, std::string file,
                 std::string directory) {
  // Busca el último "/" para extraer el nombre base del archivo .txt
  size_t posBar = file.rfind('/');
  std::string report_file;
  if (posBar != std::string::npos) {
    report_file = file.substr(posBar + 1);
    // Elimina el ".txt" para extraer únicamente el nombre del archivo
    size_t posPoint = report_file.rfind('.');
    if (posPoint != std::string::npos) {
      report_file = report_file.substr(0, posPoint);
    }
  }

  // Se cra un directorio para el job que se ejecute
  std::string report_dir = directory + "/" + report_file;
  std::string bin_dir = directory + "/" + report_file;
  if (!fs::exists(report_dir)) {
    fs::create_directory(report_dir);
  }

  // Genera el archivo .tsv del reporte final
  report_file = report_dir + "/" + report_file + ".tsv";
  std::ofstream archivo(report_file);

  if (archivo.is_open()) {
    // Guarda la información dentro del archivo .tsv
    for (size_t i = 0; i < data.size() - 1; i++) {
      archivo << data[i].bin_file << "\t" << data[i].time << "\t"
              << data[i].thermal_diffusivity << "\t" << data[i].dimensions
              << "\t" << data[i].sensitivity << "\t" << data[i].iterations
              << "\t" << data[i].format_time << std::endl;
    }

    archivo.close();

    // Genera los .bin para cada plate que contenga el .txt ejecutado
    for (size_t i = 0; i < data.size() - 1; i++) {
      std::string guionk = "-" + std::to_string(data[i].iterations);
      size_t posPoint = data[i].bin_file.rfind('.');
      std::string plateFile = data[i].bin_file;
      plateFile.insert(posPoint, guionk);
      plateFile = bin_dir + "/" + plateFile;
      std::ofstream fileLamina(plateFile,
                               std::fstream::out | std::fstream::binary);
      size_t filas = data[i].matrix.size();
      size_t columnas = (filas > 0) ? data[i].matrix[0].size() : 0;

      if (!fileLamina) {
        std::cout << "No se pudo abrir el bin_file" << std::endl;
      } else {
        fileLamina.write(reinterpret_cast<const char*>(&filas), sizeof(size_t));
        fileLamina.write(reinterpret_cast<const char*>(&columnas),
                         sizeof(size_t));

        for (const auto& fila : data[i].matrix) {
          fileLamina.write(reinterpret_cast<const char*>(fila.data()),
                           file.size() * sizeof(double));
        }

        fileLamina.close();
        std::cout << "Datos guardados exitosamente en: " << directory << std::endl;
      }
    }
  }
}

int begin2(int thread_num, int work, int thread_amount) {
  int mod = work % thread_amount;
  int min = (mod) > thread_num ? thread_num : mod;
  return thread_num * (work / thread_amount) + min;
}

int end2(int thread_num, int work, int thread_amount) {
  return begin2(thread_num + 1, work, thread_amount);
}

std::string format_time(const time_t seconds) {
  // TODO(any): Using C until C++20 std::format() is implemented by compilers
  char text[48];  // YYYY/MM/DD hh:mm:ss
  const std::tm& gmt = *std::gmtime(&seconds);
  snprintf(text, sizeof text, "%04d/%02d/%02d\t%02d:%02d:%02d",
           gmt.tm_year - 70, gmt.tm_mon, gmt.tm_mday - 1, gmt.tm_hour,
           gmt.tm_min, gmt.tm_sec);
  return text;
}
