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
	vector<pair<sword, sword>> pan_env, vol_env;
public:
	explicit Instrument(const BinInstrument&);
	
	[[nodiscard]] string get_name() const;
	[[nodiscard]] const array<byte, 96> &get_sample_keymap_assignments() const noexcept;
	[[nodiscard]] const vector<Sample> &get_samples() const noexcept;

	[[nodiscard]] vector<pair<sword, sword>> const &get_volume_envelope() const noexcept;
	[[nodiscard]] vector<pair<sword, sword>> const &get_panning_envelope() const noexcept;
	//[[nodiscard]] size_t get_number_of_volume_points() const noexcept;
	//[[nodiscard]] size_t get_number_of_panning_points() const noexcept;
	[[nodiscard]] size_t get_volume_sustain_point() const noexcept;
	[[nodiscard]] size_t get_volume_loop_start_point() const noexcept;
	[[nodiscard]] size_t get_volume_loop_end_point() const noexcept;
	[[nodiscard]] size_t get_panning_sustain_point() const noexcept;
	[[nodiscard]] size_t get_panning_loop_start_point() const noexcept;
	[[nodiscard]] size_t get_panning_loop_end_point() const noexcept;

	friend ostream &operator<<(ostream&, const Instrument&);
};

}

#endif // AWEXM_INSTRUMENT_HPP
