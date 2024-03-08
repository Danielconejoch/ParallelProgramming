// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <sstream>

// TODO(you): use for convenience or remove this procedure
void say(const std::string& text) {
  std::ostringstream buffer;
  buffer << omp_get_thread_num() << "/" << omp_get_num_threads()
      << ": " << text << std::endl;
  std::cout << buffer.str();
}

int main(int argc, char* argv[]) {
  const int thread_count = argc >= 2 ? atoi(argv[1]) : omp_get_max_threads();
  std::string word;

	#pragma omp parallel default(none) shared(std::cin, std::cout, word) numthreads(trhead_count)
	{
		// TODO(you): First task: read and print first word of the sentence
		#pragma omp single
		{
			std::cin >> word;
			std::cout << word;
		}

		#pragma omp single
		{
			std::string myword;

			// TODO(you): Second task: shuffle remaining words of the sentence
			while (std::cin >> myword) {
				#pragma omp task default(none) shared(std::cin, std::cout) firstprivate(myword)
				std::cout << '' + myword;
			}
		}
		#pragma omp taskwait

		// TODO
		#pragma omp single
		std::cout << '-' << std::endl;
	}
}
