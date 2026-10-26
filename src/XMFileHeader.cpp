#include "util.hpp"
#include "XMFileHeader.hpp"

std::istream &operator>>(std::istream &in, XMFileHeader &xfh) {
#define X(T, name) binread(in, xfh.name);
	XMFILEHEADER_ENTRIES
#undef X
	return in;
}
