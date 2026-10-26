#ifndef AWEXM_INSTRUMENT_HPP
#define AWEXM_INSTRUMENT_HPP
#include <array>
#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>
#include "SampleHeader.hpp"

namespace AweXM {

class Instrument;

class InstrumentHeader {
private:
	using char22 = std::array<char, 22>;
#define AWEXM_INSTRUMENTHEADER_ENTRIES                 \
	X(std::uint32_t,  instrument_header_size)          \
	X(char22,         instrument_name)                 \
	X(std::uint8_t,   instrument_type)                 \
	X(std::uint16_t,  number_of_samples_in_instrument)
#define X(T, name) T name{};
	AWEXM_INSTRUMENTHEADER_ENTRIES
#undef X
public:
	InstrumentHeader() = default;
	InstrumentHeader& operator=(const InstrumentHeader&) = default;
	InstrumentHeader(const InstrumentHeader&) = default;
	~InstrumentHeader() = default;
	friend std::istream &operator>>(std::istream&, Instrument&);
	friend std::istream &operator>>(std::istream&, InstrumentHeader&);
	friend std::ostream &operator<<(std::ostream&, const InstrumentHeader&);	
};

class InstrumentExtraHeader {
private:
	using byte96 = std::array<std::uint8_t, 96>;
	using byte48 = std::array<std::uint8_t, 48>;
#define AWEXM_INSTRUMENTEXTRAHEADER_ENTRIES        \
	X(std::uint32_t,  sample_header_size)          \
	X(byte96,         sample_number_for_all_notes) \
	X(byte48,         points_for_volume_envelope)  \
	X(byte48,         points_for_panning_envelope) \
	X(std::uint8_t,   number_of_volume_points)     \
	X(std::uint8_t,   number_of_panning_points)    \
	X(std::uint8_t,   volume_sustain_point)        \
	X(std::uint8_t,   volume_loop_start_point)     \
	X(std::uint8_t,   volume_loop_end_point)       \
	X(std::uint8_t,   panning_sustain_point)       \
	X(std::uint8_t,   panning_loop_start_point)    \
	X(std::uint8_t,   panning_loop_end_point)      \
	X(std::uint8_t,   volume_point)                \
	X(std::uint8_t,   panning_type)                \
	X(std::uint8_t,   vibrato_type)                \
	X(std::uint8_t,   vibrato_sweep)               \
	X(std::uint8_t,   vibrato_depth)               \
	X(std::uint8_t,   vibrato_rate)                \
	X(std::uint16_t,  volume_fadeout)              \
	X(std::uint16_t,  reserved)
#define X(T, name) T name{};
	AWEXM_INSTRUMENTEXTRAHEADER_ENTRIES
#undef X
public:
	InstrumentExtraHeader() = default;
	InstrumentExtraHeader& operator=(const InstrumentExtraHeader&) = default;
	InstrumentExtraHeader(const InstrumentExtraHeader&) = default;
	~InstrumentExtraHeader() = default;
	friend std::istream &operator>>(std::istream&, Instrument&);
	friend std::istream &operator>>(std::istream&, InstrumentExtraHeader&);
	friend std::ostream &operator<<(std::ostream&, const InstrumentExtraHeader&);
};

class Instrument {
private:
	InstrumentHeader xih;
	InstrumentExtraHeader xieh;
	std::vector<SampleHeader> v_xsh;
	std::vector<std::vector<std::uint8_t>> v_xsd;
public:
	Instrument() = default;
	Instrument& operator=(const Instrument&) = default;
	Instrument(const Instrument&) = default;
	~Instrument() = default;
	friend std::istream &operator>>(std::istream&, Instrument&);
};

}

#endif // AWEXM_INSTRUMENT_HPP
