#include "Bin.hpp"
#include "Instrument.hpp"
#include "Sample.hpp"
#include "util.hpp"

namespace AweXM {

Instrument::Instrument(const BinInstrument &i) : h(i.instrument_header) {
	for (size_t j = 0; j < i.sample_headers.size(); ++j)
		vs.emplace_back(i.sample_headers[j], i.sample_datas[j]);
}

string Instrument::get_name() const {
	return {h.instrument_name.begin(),
		std::find(h.instrument_name.begin(), h.instrument_name.end(), '\0')};
}

vector<Sample> &Instrument::get_samples() noexcept {
	return vs;
}
const vector<Sample> &Instrument::get_samples() const noexcept {
	return vs;
}
const array<byte, 96> &Instrument::get_sample_keymap_assignments() const noexcept {
	return h.sample_keymap_assignments;
}

ostream &operator<<(ostream &out, const Instrument &rhs) {
	out << "(Instrument " << rhs.get_name() << ")\n";

	out << "Samples: ";
	for (const auto &it : rhs.vs)
		out << ' ' << it;
	out << '\n';

	out << '\n';
	return out;
}

}
