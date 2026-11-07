#include "Bin.hpp"
#include "Sample.hpp"
#include "util.hpp"

namespace AweXM {

vector<sword> Sample::delta2direct8(const vector<byte> &d) {
	sbyte old = 0;
	vector<sword> ret(d.size());

	for (size_t i = 0; i < d.size(); ++i) {
		old = static_cast<sbyte>(old + d[i]);
		ret[i] = static_cast<sword>(old * 256);
	}
	return ret;
}

vector<sword> Sample::delta2direct16(const vector<byte> &d) {
	if (d.size() % 2)
		throw std::runtime_error("bad 16-bit sample size");
	
	sword old = 0;
	vector<sword> ret(d.size() / 2);
	
	for (size_t i = 0, j = 0; i < d.size(); i += 2, ++j) {
		old = static_cast<sword>(old + (d[i] | (d[i + 1] << 8)));
		ret[j] = old;
	}
	return ret;
}

vector<sword> Sample::adpcm2direct(const vector<byte> &d) const {
	byte old = 0;
	vector<sword> ret(h.sample_length);

	for (size_t i = 0, j = 0; i < ((h.sample_length + 1) >> 1); ++i, j += 2) {
		size_t idx = d[16 + i];
		old += d[idx & 0xF];
		ret[j] = static_cast<sword>(old * 256);
		if (j + 1 < ret.size()) {
			old += d[idx >> 4];
			ret[j + 1] = static_cast<sword>(old * 256);
		}
	}
	return ret;
}

Sample::Sample(BinSampleHeader const& h, vector<byte> const& data) : h(h) {
	if (get_bit_type() == bit_type::SAMPLE16)
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

Sample::bit_type Sample::get_bit_type() const noexcept {
	return (h.type & (1 << 4)) ? bit_type::SAMPLE16 : bit_type::SAMPLE8;
}

// const BinSampleHeader &Sample::get_header() const noexcept {
// 	return h;
// }

const vector<sword> &Sample::get_data() const noexcept {
	return data;
}
sbyte Sample::get_finetune() const noexcept {
	return h.finetune;
}
string Sample::get_name() const {
	return arr2str(h.sample_name);
}
byte Sample::get_panning() const noexcept {
	return h.panning;
}
sbyte Sample::get_relative_note_number() const noexcept {
	return h.relative_note_number;
}
byte Sample::get_volume() const noexcept {
	return h.volume;
}
dword Sample::get_sample_loop_start() const noexcept {
	return h.sample_loop_start;
}
dword Sample::get_sample_loop_length() const noexcept {
	return h.sample_loop_length;
}

ostream &operator<<(ostream &out, const Sample &rhs) {
	out << "(Sample " << rhs.get_name() << ": "
	    << (rhs.get_bit_type() == Sample::bit_type::SAMPLE8 ? 8 : 16) << "-bit, ";
	switch (rhs.get_loop_type()) {
		case Sample::loop_type::NO_LOOP:
			out << "No";
			break;
		case Sample::loop_type::FORWARD_LOOP:
			out << "Forward";
			break;
		case Sample::loop_type::PING_PONG_LOOP:
			out << "Ping-pong";
			break;
	}
	out << " loop)\n";

	out << "Data:";
	for (const auto &it : rhs.get_data())
		out << ' ' << it;
	out << '\n';

	out << "Finetune: " << (int)rhs.get_finetune() << '\n';
	out << "Relative note number: " << (int)rhs.get_relative_note_number() << '\n';
	out << "Panning: " << (int)rhs.get_panning() << '\n';
	out << "Volume: " << (int)rhs.get_volume() << '\n';

	out << '\n';
	return out;
}

}
