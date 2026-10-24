#include <istream>
#include <ostream>
#include "util.hpp"
#include "XMInstrument.hpp"

std::istream &operator>>(std::istream &in, XMInstrumentPt1 &xi1) {
#define X(T, name) binread(in, xi1.name);
	XMINSTRUMENTPT1_ENTRIES
#undef X
	return in;
}

std::istream &operator>>(std::istream &in, XMInstrumentPt2 &xi2) {
#define X(T, name) binread(in, xi2.name);
	XMINSTRUMENTPT2_ENTRIES
#undef X
	return in;
}

std::istream &operator>>(std::istream &in, XMInstrument &xi) {
	in >> xi.xi1;
	if (xi.xi1.number_of_samples_in_instrument)
		in >> xi.xi2;
	return in;
}
