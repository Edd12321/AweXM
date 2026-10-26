#ifndef XMINSTRUMENT_HPP
#define XMINSTRUMENT_HPP
#include <array>
#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>
#include "XMSampleHeader.hpp"

class XMInstrument;

class XMInstrumentHeader {
private:
	using char22 = std::array<char, 22>;
#define XMINSTRUMENTHEADER_ENTRIES                     \
	X(std::uint32_t,  instrument_header_size)          \
	X(char22,         instrument_name)                 \
	X(std::uint8_t,   instrument_type)                 \
	X(std::uint16_t,  number_of_samples_in_instrument)
#define X(T, name) T name{};
	XMINSTRUMENTHEADER_ENTRIES
#undef X
public:
	XMInstrumentHeader() = default;
	XMInstrumentHeader& operator=(const XMInstrumentHeader&) = default;
	XMInstrumentHeader(const XMInstrumentHeader&) = default;
	~XMInstrumentHeader() = default;
	friend std::istream &operator>>(std::istream&, XMInstrument&);
	friend std::istream &operator>>(std::istream&, XMInstrumentHeader&);
	friend std::ostream &operator<<(std::ostream&, const XMInstrumentHeader&);	
};

class XMInstrumentExtraHeader {
private:
	using byte96 = std::array<std::uint8_t, 96>;
	using byte48 = std::array<std::uint8_t, 48>;
#define XMINSTRUMENTEXTRAHEADER_ENTRIES \
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
	XMINSTRUMENTEXTRAHEADER_ENTRIES
#undef X
public:
	XMInstrumentExtraHeader() = default;
	XMInstrumentExtraHeader& operator=(const XMInstrumentExtraHeader&) = default;
	XMInstrumentExtraHeader(const XMInstrumentExtraHeader&) = default;
	~XMInstrumentExtraHeader() = default;
	friend std::istream &operator>>(std::istream&, XMInstrument&);
	friend std::istream &operator>>(std::istream&, XMInstrumentExtraHeader&);
	friend std::ostream &operator<<(std::ostream&, const XMInstrumentExtraHeader&);
};

class XMInstrument {
private:
	XMInstrumentHeader xih;
	XMInstrumentExtraHeader xieh;
	std::vector<XMSampleHeader> v_xsh;
	std::vector<std::vector<std::uint8_t>> v_xsd;
public:
	XMInstrument() = default;
	XMInstrument& operator=(const XMInstrument&) = default;
	XMInstrument(const XMInstrument&) = default;
	~XMInstrument() = default;
	friend std::istream &operator>>(std::istream&, XMInstrument&);
};

#endif // XMINSTRUMENT_HPP
