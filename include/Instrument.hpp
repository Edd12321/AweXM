#ifndef AWEXM_INSTRUMENT_HPP
#define AWEXM_INSTRUMENT_HPP
#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>
#include "SampleHeader.hpp"
#include "util.hpp"

namespace AweXM {

class Instrument;

class InstrumentHeader {
private:
#define AWEXM_INSTRUMENTHEADER_ENTRIES1                \
    X(std::uint32_t,  instrument_header_size)          \
    X(buffer<22>,     instrument_name)                 \
    X(std::uint8_t,   instrument_type)                 \
    X(std::uint16_t,  number_of_samples_in_instrument)
	std::vector<std::uint8_t> garbage;
#define AWEXM_INSTRUMENTHEADER_ENTRIES2            \
    X(std::uint32_t, sample_header_size)          \
    X(buffer<96>,    sample_number_for_all_notes) \
    X(buffer<48>,    points_for_volume_envelope)  \
    X(buffer<48>,    points_for_panning_envelope) \
    X(std::uint8_t,  number_of_volume_points)     \
    X(std::uint8_t,  number_of_panning_points)    \
    X(std::uint8_t,  volume_sustain_point)        \
    X(std::uint8_t,  volume_loop_start_point)     \
    X(std::uint8_t,  volume_loop_end_point)       \
    X(std::uint8_t,  panning_sustain_point)       \
    X(std::uint8_t,  panning_loop_start_point)    \
    X(std::uint8_t,  panning_loop_end_point)      \
    X(std::uint8_t,  volume_type)                 \
    X(std::uint8_t,  panning_type)                \
    X(std::uint8_t,  vibrato_type)                \
    X(std::uint8_t,  vibrato_sweep)               \
    X(std::uint8_t,  vibrato_depth)               \
    X(std::uint8_t,  vibrato_rate)                \
    X(std::uint16_t, volume_fadeout)              \
    X(buffer<22>,    reserved)
#define X(T, name) T name{};
	AWEXM_INSTRUMENTHEADER_ENTRIES1
	AWEXM_INSTRUMENTHEADER_ENTRIES2
#undef X
public:
	friend std::istream &operator>>(std::istream&, Instrument&);
	friend std::istream &operator>>(std::istream&, InstrumentHeader&);
	friend std::ostream &operator<<(std::ostream&, const InstrumentHeader&);	
};

class Instrument {
private:
	InstrumentHeader xih;
	std::vector<SampleHeader> v_xsh;
	std::vector<std::vector<std::uint8_t>> v_xsd;
public:
	Instrument() = default;
	Instrument& operator=(const Instrument&) = default;
	Instrument(const Instrument&) = default;
	~Instrument() = default;
	friend std::istream &operator>>(std::istream&, Instrument&);
	friend std::ostream &operator<<(std::ostream&, const Instrument&);
};

}

#endif // AWEXM_INSTRUMENT_HPP
