#ifndef XMSAMPLEHEADER_HPP
#define XMSAMPLEHEADER_HPP
#include <array>
#include <cstdint>
#include <istream>
#include <ostream>

class XMSampleHeader {
private:
	using char20 = std::array<char, 20>;
#define XMSAMPLEHEADER_ENTRIES             \
	X(std::uint32_t, sample_length)        \
	X(std::uint32_t, sample_loop_start)    \
	X(std::uint32_t, sample_loop_length)   \
	X(std::uint8_t,  volume)               \
	X(std::int8_t,   finetune)             \
	X(std::uint8_t,  type_of_sample)       \
	X(std::uint8_t,  panning)              \
	X(std::int8_t,   relative_note_number) \
	X(std::uint8_t,  reserved)             \
	X(char20,        sample_name)
#define X(T, name) T name;
	XMSAMPLEHEADER_ENTRIES
#undef X
public:
	XMSampleHeader() = default;
	XMSampleHeader& operator=(const XMSampleHeader&) = default;
	XMSampleHeader(const XMSampleHeader&) = default;
	~XMSampleHeader() = default;
	friend std::istream &operator>>(std::istream&, XMSampleHeader&);
	friend std::istream &operator>>(std::istream&, XMSampleHeader&);
	friend std::ostream &operator<<(std::ostream&, const XMSampleHeader&);
};

#endif // XMSAMPLEHEADER_HPP
