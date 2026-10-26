#include "util.hpp"
#include "XMSampleHeader.hpp"

std::istream &operator>>(std::istream &in, XMSampleHeader &xsh) {
#define X(T, name) binread(in, xsh.name);
	XMSAMPLEHEADER_ENTRIES
#undef X
	return in;
}
