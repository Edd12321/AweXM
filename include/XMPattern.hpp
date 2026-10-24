#ifndef XMPATTERN_HPP
#define XMPATTERN_HPP
#include <array>
#include <cstdint>
#include <istream>
#include <ostream>

class XMPattern {
private:
	std::uint32_t                    pattern_header_length;
	// nu stocam                     packing_type;
	std::uint16_t                    number_of_rows_in_pattern;
	std::uint16_t                    packed_patterndata_size;
	std::array<unsigned char, 65535> packed_pattern_data;
public:	
	XMPattern() = delete;
	friend std::istream &operator>>(std::istream&, XMPattern const&);
	friend std::ostream &operator<<(std::ostream&, XMPattern const&);
};

#endif // XMPATTERN_HPP
