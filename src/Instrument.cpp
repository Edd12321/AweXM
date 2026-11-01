#include "Bin.hpp"
#include "Instrument.hpp"
#include "util.hpp"

namespace AweXM {

Instrument::Instrument(BinInstrument const &i) : h(i.instrument_header) {
	vs.resize(i.sample_headers.size());
	for (size_t j = 0; j < vs.size(); ++j)
		vs[j] = Sample(i.sample_headers[j], i.sample_datas[j]);
}

}
