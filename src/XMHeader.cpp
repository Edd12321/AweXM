#include <istream>
#include <ostream>
#include "util.hpp"
#include "XMHeader.hpp"

std::istream &operator>>(std::istream &in, XMHeader &h) {
#define X(T, name) binread(in, h.name);
	XMHEADER_ENTRIES
#undef X
	return in;
}
