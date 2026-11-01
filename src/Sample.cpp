#include "Bin.hpp"
#include "Sample.hpp"
#include "util.hpp"

namespace AweXM {

vector<sword> Sample::delta2direct8(const vector<byte> &data) {
	sbyte old = 0;
	vector<sword> ret(data.size());

	for (size_t i = 0; i < data.size(); ++i) {
		old = static_cast<sbyte>(old + data[i]);
		ret[i] = static_cast<sword>(old * 256);
	}
	return ret;
}

vector<sword> Sample::delta2direct16(const vector<byte> &data) {
	if (data.size() % 2)
		throw std::runtime_error("bad 16-bit sample size");
	
	sword old = 0;
	vector<sword> ret(data.size() / 2);
	
	for (size_t i = 0, j = 0; i < data.size(); i += 2, ++j) {
		old = static_cast<sword>(old + (data[i] | (data[i + 1] << 8)));
		ret[j] = old;
	}
	return ret;
}

vector<sword> Sample::adpcm2direct(const vector<byte> &data) const {
	byte old = 0;
	vector<sword> ret(h.sample_length);

	for (size_t i = 0, j = 0; i < ((h.sample_length + 1) >> 1); ++i, j += 2) {
		size_t idx = data[16 + i];
		old += data[idx & 0xF];
		ret[j] = static_cast<sword>(old * 256);
		old += data[idx >> 4];
		ret[j + 1] = static_cast<sword>(old * 256);
	}
	return ret;
}

Sample::Sample(BinSampleHeader const& h, vector<byte> const& data) : h(h) {
	if (h.type & (1 << 4))
		this->data = delta2direct16(data);
	else if (h.delta_or_adpcm == 0xAD)
		this->data = adpcm2direct(data);
	else this->data = delta2direct8(data);
}

Sample::loop_type Sample::get_loop_type() const noexcept {
	if (h.sample_loop_length == 0)
		return loop_type::NO_LOOP;

	switch (h.type & 3) {
	case 1:
		return loop_type::FORWARD_LOOP;
	case 2: [[fallthrough]];
	case 3:
		return loop_type::PING_PONG_LOOP;
	default:
		return loop_type::NO_LOOP;
	}
}

const vector<sword> &Sample::get_data() const noexcept {
	return data;
}
sbyte Sample::get_finetune() const noexcept {
	return h.finetune;
}
const BinSampleHeader &Sample::get_header() const noexcept {
	return h;
}
string Sample::get_name() const {
	return {h.sample_name.begin(),
		std::find(h.sample_name.begin(), h.sample_name.end(), '\0')};
}
byte Sample::get_volume() const noexcept {
	return h.volume;
}

ostream &operator<<(ostream &out, const Sample &rhs) {
	out << "(Sample " << rhs.get_name() << ")\n";
	
	out << "Data:";
	for (const auto &it : rhs.get_data())
		out << ' ' << it;
	out << '\n';

	out << "Finetune:" << rhs.get_finetune() << '\n';
	out << "Volume: " << rhs.get_volume() << '\n';

	out << '\n';
	return out;
}

}
