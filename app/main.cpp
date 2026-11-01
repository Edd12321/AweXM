#include <cstdlib>
#include <iostream>

int main(int argc, char **argv) {
	if (argc == 1) {
		std::cerr << "usage: " << argv[0] << " <file>\n";
		return EXIT_SUCCESS; // EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
