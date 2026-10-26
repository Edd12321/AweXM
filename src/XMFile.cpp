#include <fstream>
#include <stdexcept>
#include "XMFile.hpp"

std::istream &operator>>(std::istream& in, XMFile& xf) {
	in >> xf.xh;
	xf.v_xp.resize(xf.xh.number_of_patterns);
	for (auto& i : xf.v_xp)
		in >> i;
	xf.v_xi.resize(xf.xh.number_of_instruments);
	for (auto& i : xf.v_xi)
		in >> i;
	return in;
}

XMFile::XMFile(std::string const& filename) {
	std::ifstream fin(filename, std::ios_base::binary);
	if (!fin)
		throw std::runtime_error("could not open file " + filename);
	fin >> *this;
}
