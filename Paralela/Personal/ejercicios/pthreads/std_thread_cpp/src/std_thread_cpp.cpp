// Copyright 2023 Daniel Conejo <daniel..conejochevez@ucr.ac.cr> CC-BY 4.0

#include "std_thread_cpp.hpp"

int main(int argc, char const *argv[])
{
	// Obtener hilos disponibles en el sistema
	uint64_t thread_count = std::thread::hardware_concurrency();

	// Si el usuario ingresa la cantidad de hilos, se usa eso
	if (argc == 2) {
		std::istringstream thread_count(argv[1]);
		if(thread_count){
    } else {
      std::cerr << "Error: invalid thread count\n";
      return EXIT_FAILURE;
    }
  }
	// Crear shared data usando smart pointers
    auto shared_data = std::make_unique<shared_data_t>();

	try{
		// Guarda el valor de thread count en shared data
		shared_data->thread_count = thread_count;

		// Registra el tiempo de inicio
 		auto start = std::chrono::high_resolution_clock::now();

		// Llama al metodo para crear los hilos
		create_threads(std::move(shared_data));

		// Registra el tiempo de finalización
		auto end = std::chrono::high_resolution_clock::now();

		// Calcula la duración de la ejecución en microsegundos
		std::chrono::microseconds duration = 
			std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			
		// Imprime la duración
		std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
	} catch (std::exception& e){
		std::cerr << e.what() << "Error: could not allocate shared data\n";
		throw e;
	}

	return 0;
}

int create_threads(std::shared_ptr<shared_data_t> shared_data){

    // Crear el vector de hilos usando std::make_unique
    auto threads = 
      std::make_unique<std::vector<std::thread>>(shared_data->thread_count);

    // Crear vector de private data usando std::make_unique
    auto private_data = 
      std::make_unique<std::vector<private_data_t>>(shared_data->thread_count);
	try{
		//asignar valores de private data
		for (size_t i = 0; i < shared_data->thread_count; i++){
			private_data->at(i).thread_number = i;
			private_data->at(i).shared_data = shared_data;
      threads->at(i) = std::thread(greet, &private_data->at(i));
		} 
    // print "Hello from main thread"
		std::cout << "Hello from main thread" << std::endl;
	
		// join threads	
		for (size_t i = 0; i < shared_data->thread_count; i++){
			threads->at(i).join();
		} 
    return EXIT_SUCCESS;
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: could not create secondary thread: " << e.what() << std::endl;
		throw e;
    return EXIT_FAILURE;
	}
};

void greet(private_data_t* data) {
  // assert(data);
  auto private_data = data;
  auto shared_data = private_data->shared_data;

  // Imprime "Hello from secondary thread"
  std::cout << "Hello from secondary thread " << private_data->thread_number
            << " of " << shared_data->thread_count << "\n";
};