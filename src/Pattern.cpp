#include "Pattern.hpp"
#include "util.hpp"

namespace AweXM {

std::istream &operator>>(std::istream &in, PatternPt1 &xpp1) {
#define X(T, data) in >> xpp1.data;
	AWEXM_PATTERNPT1_ENTRIES
#undef X
	return in;
}

std::istream &operator>>(std::istream &in, Pattern &xp) {
	in >> xp.xp1;
	if (xp.xp1.packed_patterndata_size) {
		xp.patterndata.resize(xp.xp1.packed_patterndata_size);
		binread(in, xp.patterndata);
	}
	return in;
}

}
