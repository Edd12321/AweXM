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

	[[nodiscard]] string get_id_text() const;
	[[nodiscard]] string get_module_name() const;
	[[nodiscard]] string get_tracker_name() const;
	[[nodiscard]] word get_number_of_channels() const noexcept;
	[[nodiscard]] word get_version_number() const noexcept;
	[[nodiscard]] word get_song_length() const noexcept;
	[[nodiscard]] word get_restart_position() const noexcept;
	[[nodiscard]] bool get_linear() const noexcept;

	[[nodiscard]] word get_default_tempo() const noexcept;
	[[nodiscard]] word get_default_bpm() const noexcept;

	[[nodiscard]] vector<Pattern> const &get_patterns() const noexcept;
	[[nodiscard]] vector<byte> const &get_pattern_order_table() const noexcept;
	[[nodiscard]] vector<Instrument> const &get_instruments() const noexcept;
	
	friend ostream &operator<<(ostream&, const Module&);
};

}

#endif // AWEXM_MODULE_HPP
