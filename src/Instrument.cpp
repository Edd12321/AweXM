#include "Instrument.hpp"
#include "util.hpp"

namespace AweXM {

std::istream &operator>>(std::istream &in, InstrumentHeader &xih) {
#define X(T, name) binread(in, xih.name);
	AWEXM_INSTRUMENTHEADER_ENTRIES
#undef X
	return in;
}

std::istream &operator>>(std::istream &in, InstrumentExtraHeader &xieh) {
#define X(T, name) binread(in, xieh.name);
	AWEXM_INSTRUMENTEXTRAHEADER_ENTRIES
#undef X
	return in;
}

std::istream &operator>>(std::istream &in, Instrument &xi) {
	in >> xi.xih;
	auto ns = xi.xih.number_of_samples_in_instrument;
	if (ns > 0) {
		in >> xi.xieh;
		xi.v_xsh.resize(ns);
		xi.v_xsd.resize(ns);
		for (auto &i : xi.v_xsh)
			in >> i;
		for (std::size_t i = 0; i < ns; ++i) {
			xi.v_xsd[i].resize(xi.v_xsh[i].sample_length);
			binread(in, xi.v_xsd[i]);
		}
	}
	return in;
}

}
