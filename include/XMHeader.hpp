#ifndef XMHEADER_HPP
#define XMHEADER_HPP
#include <array>
#include <cstdint>
#include <istream>
#include <ostream>

class XMHeader {
private:
	// nu stocam                  id_text;
	std::array<char, 20>          module_name;
	std::array<char, 20>          tracker_name;
	std::array<std::uint8_t, 2>   version_number;
	std::uint32_t                 header_size;
	std::uint16_t                 song_length;
	std::uint16_t                 song_restart_position;
	std::uint16_t                 number_of_channels;
	std::uint16_t                 number_of_patterns;
	std::uint16_t                 number_of_instruments;
	bool                          flags_field;
	std::uint16_t                 default_tempo;
	std::uint16_t                 default_bpm;
	std::array<std::uint8_t, 256> pattern_order_table;
public:
	XMHeader() = delete;
	friend std::istream &operator>>(std::istream&, XMHeader const&);
	friend std::ostream &operator<<(std::ostream&, XMHeader const&);
};

#endif // XMHEADER_HPP
