#include "Pattern.hpp"
#include "util.hpp"

namespace AweXM {

std::istream &operator>>(std::istream &in, Pattern &xp) {
#define X(T, name) binread(in, xp.name);
	AWEXM_PATTERN_ENTRIES1
#undef X
	auto ps = xp.packed_patterndata_size;
	if (ps) {
		xp.patterndata.resize(ps);
		binread(in, xp.patterndata);
	}
	return in;
}
std::ostream &operator<<(std::ostream &out, const Pattern &xp) {
#define X(T, name) binwrite(out, xp.name);
	AWEXM_PATTERN_ENTRIES1
#undef X
	binwrite(out, xp.patterndata);
	return out;
}

}
