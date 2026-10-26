#include "util.hpp"
#include "XMInstrument.hpp"

std::istream &operator>>(std::istream &in, XMInstrumentHeader &xih) {
#define X(T, name) binread(in, xih.name);
	XMINSTRUMENTHEADER_ENTRIES
#undef X
	return in;
}

std::istream &operator>>(std::istream &in, XMInstrumentExtraHeader &xieh) {
#define X(T, name) binread(in, xieh.name);
	XMINSTRUMENTEXTRAHEADER_ENTRIES
#undef X
	return in;
}

std::istream &operator>>(std::istream &in, XMInstrument &xi) {
	in >> xi.xih;
	auto ns = xi.xih.number_of_samples_in_instrument;
	if (ns > 0) {
		in >> xi.xieh;
		xi.v_xsh.resize(ns);
		xi.v_xsd.resize(ns);
		for (auto &i : xi.v_xsh)
			in >> i;
		for (auto &i : xi.v_xsd)
			binread(in, i);
	}
	return in;
}
