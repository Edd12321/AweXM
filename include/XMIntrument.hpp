#ifndef XMINSTRUMENT_HPP
#define XMINSTRUMENT_HPP
#include <array>
#include <cstdint>
#include <istream>
#include <ostream>

class XMInstrument {
private:
	std::uint32_t                instrument_header_size;
	std::array<char, 22>         instrument_name;
	// nu stocam                 instrument_type;
	std::uint16_t                number_of_samples_in_instrument;
	// daca numarul de sampleuri e >0 atunci urmeaza asta
	// daca nu atunci nu
	std::uint32_t                sample_header_size;
	std::array<std::uint8_t, 96> sample_number_for_all_notes;
	std::array<std::uint8_t, 48> points_for_volume_envelope;
	std::array<std::uint8_t, 48> points_for_panning_envelope;
	std::uint8_t                 number_of_volume_points;
	std::uint8_t                 number_of_panning_points;
	std::uint8_t                 volume_sustain_point;
	std::uint8_t                 volume_loop_start_point;
	std::uint8_t                 volume_loop_end_point;
	std::uint8_t                 panning_sustain_point;
	std::uint8_t                 panning_loop_start_point;
	std::uint8_t                 panning_loop_end_point;
	std::uint8_t                 volume_point;
	std::uint8_t                 panning_type;
	std::uint8_t                 vibrato_type;
	std::uint8_t                 vibrato_sweep;
	std::uint8_t                 vibrato_depth;
	std::uint8_t                 vibrato_rate;
	std::uint16_t                volume_fadeout;
	std::uint16_t                reserved; // ?
public:
	XMInstrument() = delete;
	friend std::istream &operator>>(std::istream&, XMInstrument const&);
	friend std::ostream &operator<<(std::ostream&, XMInstrument const&);
};

#endif // XMINSTRUMENT_HPP
