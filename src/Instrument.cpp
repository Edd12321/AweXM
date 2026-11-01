#include "Bin.hpp"
#include "Instrument.hpp"
#include "util.hpp"

namespace AweXM {

Instrument::Instrument(BinInstrument const &i) : h(i.instrument_header) {
	for (size_t j = 0; j < i.sample_headers.size(); ++j)
		vs.emplace_back(i.sample_headers[j], i.sample_datas[j]);
}

string Instrument::get_name() const noexcept {
	return {h.instrument_name.begin(),
		std::find(h.instrument_name.begin(), h.instrument_name.end(), '\0')};
}

vector<Sample> &Instrument::get_samples() noexcept {
	return vs;
}
const vector<Sample> &Instrument::get_samples() const noexcept {
	return vs;
}

ostream &operator<<(ostream &out, const Instrument &rhs) {
	out << "(Instrument " << rhs.get_name() << ")\n";
	for (const auto &it : rhs.get_samples())
		out << it << '\n';

	out << '\n';
	return out;
}

}
