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
	for (const auto &it : rhs.get_patterns())
		out << it << '\n';
	out << "\nINSTRUMENTS\n-----------\n";
	for (const auto &it : rhs.get_instruments())
		out << it << '\n';
	return out;
}

}
