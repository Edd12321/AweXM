#include "util.hpp"
#include "XMPattern.hpp"

std::istream &operator>>(std::istream &in, XMPattern &xp) {
	in >> xp.xp1;
	if (xp.xp1.packed_patterndata_size) {
		xp.patterndata.resize(xp.xp1.packed_patterndata_size);
		binread(in, xp.patterndata);
	}
	return in;
}
