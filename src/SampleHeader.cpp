#include "SampleHeader.hpp"
#include "util.hpp"

namespace AweXM {

std::istream &operator>>(std::istream &in, SampleHeader &xsh) {
#define X(T, name) binread(in, xsh.name);
	AWEXM_SAMPLEHEADER_ENTRIES
#undef X
	return in;
}
std::ostream &operator<<(std::ostream &out, const SampleHeader &xsh) {
#define X(T, name) binwrite(out, xsh.name);
	AWEXM_SAMPLEHEADER_ENTRIES
#undef X
	return out;
}

}
