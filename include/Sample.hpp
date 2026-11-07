#ifndef AWEXM_SAMPLE_HPP
#define AWEXM_SAMPLE_HPP
#include "Bin.hpp"
#include "util.hpp"

namespace AweXM {

class Sample {
private:
	BinSampleHeader h;
	vector<sword> data;

	static vector<sword> delta2direct8(const vector<byte> &data);
	static vector<sword> delta2direct16(const vector<byte> &data);
	[[nodiscard]] vector<sword> adpcm2direct(const vector<byte> &data) const;
public:
	enum class loop_type {
		NO_LOOP,
		FORWARD_LOOP,
		PING_PONG_LOOP
	};
	enum class bit_type {
		SAMPLE8,
		SAMPLE16
	};

	Sample() = default;
	Sample(Sample const&) = default;
	Sample& operator=(Sample const&) = default;
	Sample(Sample&&) = default;
	Sample& operator=(Sample&&) = default;
	explicit Sample(BinSampleHeader const&, vector<byte> const&);
	
	// [[nodiscard]] const BinSampleHeader &get_header() const noexcept;
	[[nodiscard]] bit_type get_bit_type() const noexcept;
	[[nodiscard]] loop_type get_loop_type() const noexcept;

	[[nodiscard]] dword get_sample_loop_start() const noexcept;
	[[nodiscard]] dword get_sample_loop_length() const noexcept;
	[[nodiscard]] const vector<sword> &get_data() const noexcept;
	[[nodiscard]] sbyte get_finetune() const noexcept;
	[[nodiscard]] sbyte get_relative_note_number() const noexcept;
	[[nodiscard]] byte get_panning() const noexcept;
	[[nodiscard]] byte get_volume() const noexcept;
	[[nodiscard]] string get_name() const;
	
	friend ostream &operator<<(ostream&, const Sample&);
};

}

#endif // AWEXM_SAMPLE_HPP
