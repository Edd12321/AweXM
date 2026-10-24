#ifndef XMINSTRUMENT_HPP
#define XMINSTRUMENT_HPP
#include <array>
#include <cstdint>
#include <istream>
#include <ostream>

class XMInstrumentPt1;
class XMInstrumentPt2;
class XMInstrument;

class XMInstrumentPt1 {
private:
	using char22 = std::array<char, 22>;
#define XMINSTRUMENTPT1_ENTRIES                        \
	X(std::uint32_t,  instrument_header_size)          \
	X(char22,         instrument_name)                 \
	X(std::uint8_t,   instrument_type)                 \
	X(std::uint16_t,  number_of_samples_in_instrument)
#define X(T, name) T name;
	XMINSTRUMENTPT1_ENTRIES
#undef X
public:
	XMInstrumentPt1(XMInstrumentPt1 const& rhs) {
#define X(T, name) name = rhs.name;
		XMINSTRUMENTPT1_ENTRIES
#undef X
	}
	friend std::istream &operator>>(std::istream&, XMInstrument&);
	friend std::istream &operator>>(std::istream&, XMInstrumentPt1&);
	friend std::ostream &operator<<(std::ostream&, XMInstrumentPt1 const&);	
};

class XMInstrumentPt2 {
private:
	using byte96 = std::array<std::uint8_t, 96>;
	using byte48 = std::array<std::uint8_t, 48>;
#define XMINSTRUMENTPT2_ENTRIES \
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
#define X(T, name) T name;
	XMINSTRUMENTPT2_ENTRIES
#undef X
public:
	XMInstrumentPt2(XMInstrumentPt2 const& rhs) {
#define X(T, name) name = rhs.name;
		XMINSTRUMENTPT2_ENTRIES
#undef X
	}
	friend std::istream &operator>>(std::istream&, XMInstrument&);
	friend std::istream &operator>>(std::istream&, XMInstrumentPt2&);
	friend std::ostream &operator<<(std::ostream&, XMInstrumentPt2 const&);
};

class XMInstrument {
private:
	XMInstrumentPt1 xi1;
	XMInstrumentPt2 xi2;
public:
	friend std::istream &operator>>(std::istream&, XMInstrument&);
};

#endif // XMINSTRUMENT_HPP
