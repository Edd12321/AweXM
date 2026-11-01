#ifndef AWEXM_MODULE_HPP
#define AWEXM_MODULE_HPP
#include "Bin.hpp"
#include "Instrument.hpp"
#include "Pattern.hpp"
#include "util.hpp"

namespace AweXM {

class Module {
private:
	BinXMHeader h;
	vector<Pattern> pats;
	vector<Instrument> instrs;
public:
	explicit Module(const BinModule&);
	explicit Module(const std::string&);

	[[nodiscard]] string get_id_text() const noexcept        { return arr2str(h.id_text); }
	[[nodiscard]] string get_module_name() const noexcept    { return arr2str(h.module_name); }
	[[nodiscard]] string get_tracker_name() const noexcept   { return arr2str(h.tracker_name); }
	[[nodiscard]] word get_version_number() const noexcept   { return h.version_number; }
	[[nodiscard]] word get_song_length() const noexcept      { return h.song_length; }
	[[nodiscard]] word get_restart_position() const noexcept { return h.restart_position; }

	[[nodiscard]] word get_default_tempo() const noexcept    { return h.default_tempo; }
	[[nodiscard]] word get_default_bpm() const noexcept      { return h.default_bpm; }

	[[nodiscard]] vector<Pattern> const &get_patterns() const noexcept {
		return pats;
	}
	[[nodiscard]] vector<Instrument> const &get_instruments() const noexcept {
		return instrs;
	}
	friend ostream &operator<<(ostream&, const Module&);
};

}

#endif // AWEXM_MODULE_HPP
