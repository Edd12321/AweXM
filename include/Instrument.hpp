#ifndef AWEXM_INSTRUMENT_HPP
#define AWEXM_INSTRUMENT_HPP
#include "Bin.hpp"
#include "Sample.hpp"
#include "util.hpp"

namespace AweXM {

class Instrument {
private:
	BinInstrumentHeader h;
	vector<Sample> vs;
public:
	explicit Instrument(BinInstrument const&);
};

}

#endif // AWEXM_INSTRUMENT_HPP
