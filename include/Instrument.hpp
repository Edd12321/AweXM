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
	explicit Instrument(const BinInstrument&);
	
	[[nodiscard]] string get_name() const;
	[[nodiscard]] const array<byte, 96> &get_sample_keymap_assignments() const noexcept;
	[[nodiscard]] const vector<Sample> &get_samples() const noexcept;
	vector<Sample> &get_samples() noexcept;

	friend ostream &operator<<(ostream&, const Instrument&);
};

}

#endif // AWEXM_INSTRUMENT_HPP
