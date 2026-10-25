#ifndef XMPATTERN_HPP
#define XMPATTERN_HPP
#include <cstdint>
#include <istream>
#include <ostream>
#include <vector>

class XMPatternPt1;
class XMPattern;

class XMPatternPt1 {
private:
#define XMPATTERNPT1_ENTRIES                    \
	X(std::uint32_t, pattern_header_length)     \
	X(std::uint8_t,  packing_type)              \
	X(std::uint16_t, number_of_rows_in_pattern) \
	X(std::uint16_t, packed_patterndata_size)
#define X(T, name) T name;
	XMPATTERNPT1_ENTRIES
#undef X
public:	
	XMPatternPt1& operator=(const XMPatternPt1&) = default;
	XMPatternPt1(const XMPatternPt1&) = default;
	~XMPatternPt1() = default;
	friend std::istream &operator>>(std::istream&, XMPattern&);
	friend std::istream &operator>>(std::istream&, XMPatternPt1&);
	friend std::ostream &operator<<(std::ostream&, const XMPatternPt1&);
};

class XMPattern {
private:
	XMPatternPt1 xp1;
	std::vector<std::uint8_t> patterndata;
public:
	XMPattern& operator=(const XMPattern& rhs) = default;
	XMPattern(const XMPattern &rhs) = default;
	~XMPattern() = default;
	friend std::istream &operator>>(std::istream&, XMPattern&);
	friend std::ostream &operator<<(std::ostream&, const XMPattern&);
};

#endif // XMPATTERN_HPP
