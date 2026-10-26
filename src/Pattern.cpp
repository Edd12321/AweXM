#include "Pattern.hpp"
#include "util.hpp"

namespace AweXM {

std::istream &operator>>(std::istream &in, Pattern &xp) {
	in >> xp.xp1;
	if (xp.xp1.packed_patterndata_size) {
		xp.patterndata.resize(xp.xp1.packed_patterndata_size);
		binread(in, xp.patterndata);
	}
	return in;
}

}
