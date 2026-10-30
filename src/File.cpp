#include <fstream>
#include <stdexcept>
#include <utility>
#include "File.hpp"
#include "util.hpp"

namespace AweXM {

std::istream &operator>>(std::istream &in, FileHeader &xfh) {
#define X(T, name) binread(in, xfh.name);
	AWEXM_FILEHEADER_ENTRIES
#undef X
	return in;
}
std::ostream &operator<<(std::ostream &out, const FileHeader &xfh) {
#define X(T, name) binwrite(out, xfh.name);
	AWEXM_FILEHEADER_ENTRIES
#undef X
	return out;
}

std::istream &operator>>(std::istream &in, File &xf) {
	in >> xf.xh;
	xf.v_xp.resize(xf.xh.number_of_patterns);
	for (auto& i : xf.v_xp)
		in >> i;
	xf.v_xi.resize(xf.xh.number_of_instruments);
	for (auto& i : xf.v_xi)
		in >> i;
	return in;
}
std::ostream &operator<<(std::ostream &out, const File &xf) {
	out << xf.xh;
	binwrite(out, xf.v_xp);
	binwrite(out, xf.v_xi);
	return out;
}

File::File(std::string const& filename) {
	std::ifstream fin(filename, std::ios_base::binary);
	if (!fin)
		throw std::runtime_error("could not open file " + filename);
	fin >> *this;
}

File &File::operator=(const File &rhs) {
	if (this != &rhs) {
		xh = rhs.xh;
		v_xp = rhs.v_xp;
		v_xi = rhs.v_xi;
	}
	return *this;
}

File::File(const File &rhs) {
	*this = rhs;
}


File &File::operator=(File &&rhs) noexcept {
	if (this != &rhs) {
		xh = rhs.xh;
		v_xp = std::move(rhs.v_xp);
		v_xi = std::move(rhs.v_xi);
	}
	return *this;
}

File::File(File &&rhs) noexcept {
	*this = std::move(rhs);
}

File::~File() {
	volatile char c;
	(void)(c = 0); // trebuia sa faca ceva
}

}
