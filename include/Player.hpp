#ifndef AWEXM_PLAYER_HPP
#define AWEXM_PLAYER_HPP
#include "Module.hpp"
#include "Pattern.hpp"
#include "util.hpp"

namespace AweXM {

class Player {
private:
	const Module *m = nullptr;

	size_t tick{};
	// word   pattern{};
	word   row{};
	byte   song_pos{};
	byte   new_song_pos{};

	word   bpm{};
	word   tempo{};

	bool   first_tick{true};
	bool   pattern_break{false};
	bool   row_jump{false};
	byte   row_jump_target{};
	byte   repeat_row{};

	struct ChannelState {
		// efect curent
		Pattern::Cell cell;
		bool active{false};
		word memory['Z' + 1] = {};

		word sample_pos{};
	};
	vector<ChannelState> chst;
public:
	void load(const Module &md) {
		m = &md;
		bpm = md.get_default_bpm();
		tempo = md.get_default_tempo();
		tick = row = song_pos = 0;
		chst.resize(md.get_number_of_channels());
	}
	void advance();
	static void process_cell_tick0(const Pattern::Cell&, ChannelState&, char, byte, byte, byte);
	static void process_cell_rest(const Pattern::Cell&, ChannelState&, char, byte, byte, byte);
};

}

#endif // AWEXM_PLAYER_HPP
