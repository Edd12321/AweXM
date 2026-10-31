#include "Instrument.hpp"
#include "util.hpp"
#include <iostream>

namespace AweXM {

std::istream &operator>>(std::istream &in, InstrumentHeader &xih) {
#define X(T, name) binread(in, xih.name);
	AWEXM_INSTRUMENTHEADER_ENTRIES1
	if (xih.number_of_samples_in_instrument) {
		AWEXM_INSTRUMENTHEADER_ENTRIES2
	} else if (xih.instrument_header_size > 29) {
		xih.garbage.resize(xih.instrument_header_size - 29);
		binread(in, xih.garbage);
	}
#undef X
	return in;
}
std::ostream &operator<<(std::ostream &out, const InstrumentHeader &xih) {
#define X(T, name) binwrite(out, xih.name);
	AWEXM_INSTRUMENTHEADER_ENTRIES1
	if (xih.number_of_samples_in_instrument) {
		AWEXM_INSTRUMENTHEADER_ENTRIES2
	} else if (xih.instrument_header_size > 29)
		binwrite(out, xih.garbage);
#undef X
	return out;
}

std::istream &operator>>(std::istream &in, Instrument &xi) {
	in >> xi.xih;
	auto ns = xi.xih.number_of_samples_in_instrument;
	if (ns > 0) {
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
std::ostream &operator<<(std::ostream &out, const Instrument &xi) {
	out << xi.xih;
	for (const auto &i : xi.v_xsh)
		out << i;
	for (const auto &i : xi.v_xsd)
		binwrite(out, i);
	return out;
}

}
