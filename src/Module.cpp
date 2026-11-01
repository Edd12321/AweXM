#include "Module.hpp"
#include "Bin.hpp"
#include "util.hpp"

namespace AweXM {

Module::Module(const BinModule &m) : h(m.xm_header) {
	for (size_t i = 0; i < h.number_of_patterns; ++i)
		pats.emplace_back(h, m.patterns[i]);
	for (size_t i = 0; i < h.number_of_instruments; ++i)
		instrs.emplace_back(m.instruments[i]);
}

Module::Module(const std::string &filename) {
	std::ifstream fin(filename, std::ios_base::binary);
	if (!fin)
		throw std::runtime_error("could not open file " + filename);
	
	BinModule bm;
	read_bin_module(fin, bm);
	*this = Module(bm);
}

}
