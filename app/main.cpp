#include <cstdlib>
#include <iostream>
#include "Module.hpp"
#include "util.hpp"

int main(int argc, char **argv) {
	if (argc == 1) {
		std::cerr << "usage: " << argv[0] << " <file>\n";
		return EXIT_SUCCESS; // EXIT_FAILURE;
	}
	AweXM::Module mod(argv[1]);
	std::cout << mod << '\n';
	return EXIT_SUCCESS;
}
