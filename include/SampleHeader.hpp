#ifndef AWEXM_SAMPLEHEADER_HPP
#define AWEXM_SAMPLEHEADER_HPP
#include <array>
#include <cstdint>
#include <istream>
#include <ostream>

namespace AweXM {

class SampleHeader {
private:
	using char20 = std::array<char, 20>;
#define AWEXM_SAMPLEHEADER_ENTRIES         \
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
	AWEXM_SAMPLEHEADER_ENTRIES
#undef X
public:
	SampleHeader() = default;
	SampleHeader& operator=(const SampleHeader&) = default;
	SampleHeader(const SampleHeader&) = default;
	~SampleHeader() = default;
	friend std::istream &operator>>(std::istream&, SampleHeader&);
	friend std::istream &operator>>(std::istream&, SampleHeader&);
	friend std::ostream &operator<<(std::ostream&, const SampleHeader&);
};

}

#endif // AWEXM_SAMPLEHEADER_HPP
