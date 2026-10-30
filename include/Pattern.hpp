#ifndef AWEXM_PATTERN_HPP
#define AWEXM_PATTERN_HPP
#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>

namespace AweXM {

class Pattern {
private:
#define AWEXM_PATTERN_ENTRIES1                  \
    X(std::uint32_t, pattern_header_length)     \
    X(std::uint8_t,  packing_type)              \
    X(std::uint16_t, number_of_rows_in_pattern) \
    X(std::uint16_t, packed_patterndata_size)
#define X(T, name) T name{};
	AWEXM_PATTERN_ENTRIES1
#undef X
	std::vector<std::uint8_t> patterndata;
public:
	friend std::istream &operator>>(std::istream&, Pattern&);
	friend std::ostream &operator<<(std::ostream&, const Pattern&);
};

}

#endif // XMPATTERN_HPP
