#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Module.hpp"
#include "Player.hpp"

int main(int argc, char **argv) {
	try {
		char opts[1000] = "";
#define DEFARG(NAME) bool NAME = argc > 1 && !std::strcmp(argv[1], #NAME); \
                     std::strcat(opts, #NAME"|");
		DEFARG(dump)
		DEFARG(raw)
		DEFARG(nice)
		opts[std::strlen(opts) - 1] = '\0';
		if (argc == 3 && dump) {
			AweXM::Module mod(argv[2]);
			std::cout << mod << '\n';
		} else if (argc == 3 && (raw || nice)) {
			AweXM::Module mod(argv[2]);
			AweXM::Player p;
			p.load(mod);
			p.play(512, 48000, nice);
		} else {
			std::cerr << "usage: " << argv[0] << ' ' << opts << " <file>\n";
			return EXIT_SUCCESS; // EXIT_FAILURE;
		}
	} catch (const std::exception &err) {
		std::cerr << "Program threw exception of type " << typeid(err).name()
		          << " and message what(): " << err.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
