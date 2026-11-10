#include "Module.hpp"
#include "Bin.hpp"
#include "util.hpp"

namespace AweXM {

Module::Module(const BinModule &m) : h(m.xm_header) {
	for (size_t i = 0; i < h.number_of_patterns; ++i)
		pats.emplace_back(h, m.patterns[i]);
	for (size_t i = 0; i < h.number_of_instruments; ++i)
		instrs.emplace_back(m.instruments[i]);
}

Module::Module(const std::string &filename) {
	std::ifstream fin(filename, std::ios_base::binary);
	if (!fin)
		throw std::runtime_error("could not open file " + filename);
	
	BinModule bm;
	read_bin_module(fin, bm);
	*this = Module(bm);
}

ostream &operator<<(ostream &out, const Module &rhs) {
	out << "(Module " << rhs.get_module_name() << ")\n";
	out << "ID text: " << rhs.get_id_text() << '\n';
	out << "Tracker: " << rhs.get_tracker_name() << '\n';
	out << "Version: " << rhs.get_version_number() << '\n';
	out << "Song length: " << rhs.get_song_length() << '\n';
	out << "Restart position: " << rhs.get_restart_position() << '\n';
	out << "Default tempo: " << rhs.get_default_tempo() << '\n';
	out << "Default BPM: " << rhs.get_default_bpm() << '\n';
	out << "PATTERNS\n--------\n";
	size_t k = 0;
	for (const auto &it : rhs.pats)
		out << "(Pattern #" << ++k << ")\n" << it << '\n';
	out << "\nINSTRUMENTS\n-----------\n";
	k = 0;
	for (const auto &it : rhs.instrs)
		out << "(Instrument #" << ++k << ")\n" << it << '\n';
	return out;
}

string Module::get_id_text() const {
	return arr2str(h.id_text);
}
string Module::get_module_name() const {
	return arr2str(h.module_name);
}
string Module::get_tracker_name() const {
	return arr2str(h.tracker_name);
}
word Module::get_number_of_channels() const noexcept {
	return h.number_of_channels;
}
word Module::get_version_number() const noexcept {
	return h.version_number;
}
word Module::get_song_length() const noexcept {
	return h.song_length;
}
word Module::get_restart_position() const noexcept {
	return h.restart_position;
}
bool Module::get_linear() const noexcept {
	return h.flags & 1;
}
word Module::get_default_tempo() const noexcept {
	return h.default_tempo;
}
word Module::get_default_bpm() const noexcept {
	return h.default_bpm;
}
vector<Pattern> const &Module::get_patterns() const noexcept {
	return pats;
}
vector<byte> const &Module::get_pattern_order_table() const noexcept {
	return h.pattern_order_table;
}
vector<Instrument> const &Module::get_instruments() const noexcept {
	return instrs;
}

}
