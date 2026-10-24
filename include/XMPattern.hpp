#ifndef XMPATTERN_HPP
#define XMPATTERN_HPP
#include <array>
#include <cstdint>
#include <istream>
#include <ostream>

class XMPattern {
private:
	using byte65535 = std::array<unsigned char, 65535>;
#define XMPATTERN_ENTRIES                       \
	X(std::uint32_t, pattern_header_length)     \
	X(std::uint8_t,  packing_type)              \
	X(std::uint16_t, number_of_rows_in_pattern) \
	X(std::uint16_t, packed_patterndata_size)   \
	X(byte65535,     packed_pattern_data)
#define X(T, name) T name;
	XMPATTERN_ENTRIES
#undef X
public:	
	XMPattern(XMPattern const& rhs) {
#define X(T, name) name = rhs.name;
		XMPATTERN_ENTRIES
#undef X
	}
	friend std::istream &operator>>(std::istream&, XMPattern&);
	friend std::ostream &operator<<(std::ostream&, XMPattern const&);
};

#endif // XMPATTERN_HPP
