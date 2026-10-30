#include <iostream>
#include <cstdlib>
#include "File.hpp"

int main(int argc, char **argv) {
	if (argc == 1) {
		std::cerr << "usage: " << argv[0] << " <file>\n";
		return EXIT_SUCCESS; // EXIT_FAILURE;
	}
	AweXM::File f(argv[1]);
	std::cout << f;
	return EXIT_SUCCESS;
}
