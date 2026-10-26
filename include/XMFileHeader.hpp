#ifndef XMHEADER_HPP
#define XMHEADER_HPP
#include <array>
#include <cstdint>
#include <istream>
#include <ostream>

class XMFile;

class XMFileHeader {
private:
	using char17  = std::array<char, 17>;
	using char20  = std::array<char, 20>;
	using byte256 = std::array<std::uint8_t, 256>;
#define XMFILEHEADER_ENTRIES                \
	X(char17,        id_text)               \
	X(char20,        module_name)           \
	X(std::uint8_t,  always_1a)             \
	X(char20,        tracker_name)          \
	X(std::uint16_t, version_number)        \
	X(std::uint32_t, header_size)           \
	X(std::uint16_t, song_length)           \
	X(std::uint16_t, song_restart_position) \
	X(std::uint16_t, number_of_channels)    \
	X(std::uint16_t, number_of_patterns)    \
	X(std::uint16_t, number_of_instruments) \
	X(std::uint16_t, flags_field)           \
	X(std::uint16_t, default_tempo)         \
	X(std::uint16_t, default_bpm)           \
	X(byte256,       pattern_order_table)
#define X(T, name) T name{};
	XMFILEHEADER_ENTRIES
#undef X
public:
	XMFileHeader() = default;
	XMFileHeader& operator=(const XMFileHeader& rhs) = default;
	XMFileHeader(const XMFileHeader &rhs) = default;
	~XMFileHeader() = default;
	friend std::istream &operator>>(std::istream&, XMFileHeader&);
	friend std::istream &operator>>(std::istream&, XMFile&);
	friend std::ostream &operator<<(std::ostream&, const XMFileHeader&);
};

#endif // XMHEADER_HPP
