#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Module.hpp"
#include "Player.hpp"

int main(int argc, char **argv) {
	try {
		if (argc == 3 && !std::strcmp(argv[1], "dump")) {
			AweXM::Module mod(argv[2]);
			std::cout << mod << '\n';
		} else if (argc == 3 && !std::strcmp(argv[1], "play")) {
			AweXM::Module mod(argv[2]);
			AweXM::Player p;
			p.load(mod);
			p.play();
		} else {
			std::cerr << "usage: dump|play " << argv[0] << " <file>\n";
			return EXIT_SUCCESS; // EXIT_FAILURE;
		}
	} catch (const std::exception &err) {
		std::cerr << "Program threw exception of type " << typeid(err).name()
		          << " and message what(): " << err.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
