#include "Bin.hpp"
#include "Instrument.hpp"
#include "Sample.hpp"
#include "util.hpp"

namespace AweXM {

Instrument::Instrument(const BinInstrument &i) : h(i.instrument_header) {
	for (size_t j = 0; j < i.sample_headers.size(); ++j)
		vs.emplace_back(i.sample_headers[j], i.sample_datas[j]);

	int np = h.number_of_panning_points;
	int nv = h.number_of_volume_points;
	pan_env.resize(np);
	vol_env.resize(nv);
	const auto &P = h.points_for_panning_envelope;
	const auto &V = h.points_for_volume_envelope;
	for (int j = 0; j < std::min(12, np); ++j)
		pan_env[j] = {P[2 * j], P[2 * j + 1]};
	for (int j = 0; j < std::min(12, nv); ++j)
		vol_env[j] = {V[2 * j], V[2 * j + 1]};
}

string Instrument::get_name() const {
	return {h.instrument_name.begin(),
		std::find(h.instrument_name.begin(), h.instrument_name.end(), '\0')};
}

const vector<Sample> &Instrument::get_samples() const noexcept {
	return vs;
}
const array<byte, 96> &Instrument::get_sample_keymap_assignments() const noexcept {
	return h.sample_keymap_assignments;
}
vector<pair<sword, sword>> const &Instrument::get_volume_envelope() const noexcept {
	return vol_env;
}
vector<pair<sword, sword>> const &Instrument::get_panning_envelope() const noexcept {
	return pan_env;
}
/*
size_t Instrument::get_number_of_volume_points() const noexcept {
	return h.number_of_volume_points;
}
size_t Instrument::get_number_of_panning_points() const noexcept {
	return h.number_of_panning_points;
}
*/
size_t Instrument::get_volume_sustain_point() const noexcept {
	return h.volume_sustain_point;
}
size_t Instrument::get_volume_loop_start_point() const noexcept {
	return h.volume_loop_start_point;
}
size_t Instrument::get_volume_loop_end_point() const noexcept {
	return h.volume_loop_end_point;
}
size_t Instrument::get_panning_sustain_point() const noexcept {
	return h.volume_sustain_point;
}
size_t Instrument::get_panning_loop_start_point() const noexcept {
	return h.volume_loop_start_point;
}
size_t Instrument::get_panning_loop_end_point() const noexcept {
	return h.volume_loop_end_point;
}

ostream &operator<<(ostream &out, const Instrument &rhs) {
	out << "(Instrument " << rhs.get_name() << ")\n";

	out << "Panning Env:";
	for (const auto &it : rhs.get_panning_envelope())
		out << " (" << it.first << ", " << it.second << ')';
	out << '\n';
	out << "Panning sustain point: " << rhs.get_panning_sustain_point() << '\n';
	out << "Panning loop start point: " << rhs.get_panning_loop_start_point() << '\n';
	out << "Panning loop end point: " << rhs.get_panning_loop_end_point() << '\n';
	
	out << "Volume Env:";
	for (const auto &it : rhs.get_volume_envelope())
		out << " (" << it.first << ", " << it.second << ')';
	out << '\n';
	out << "Volume sustain point: " << rhs.get_volume_sustain_point() << '\n';
	out << "Volume loop start point: " << rhs.get_volume_loop_start_point() << '\n';
	out << "Volume loop end point: " << rhs.get_volume_loop_end_point() << '\n';

	out << "Non-default sample keymap assignments:";
	const auto &ka = rhs.get_sample_keymap_assignments();
	for (size_t i = 0; i < 96; ++i)
		if (ka[i])
			out << ' ' << note2str(i + 1) << "->" << static_cast<int>(ka[i]);
	out << '\n';

	out << "Samples: ";
	for (const auto &it : rhs.vs)
		out << ' ' << it;
	out << '\n';

	out << '\n';
	return out;
}

}
