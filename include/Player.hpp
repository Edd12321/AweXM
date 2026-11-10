#ifndef AWEXM_PLAYER_HPP
#define AWEXM_PLAYER_HPP
#include "Module.hpp"
#include "Pattern.hpp"
#include "util.hpp"

namespace AweXM {

class Player {
private:
	const Module *m = nullptr;
	double frames_til_tick{};

	size_t tick{};
	// word   pattern{};
	word   row{};
	byte   song_pos{};
	byte   new_song_pos{1};

	word   bpm{};
	word   tempo{};
	word   global_volume{64};

	bool   first_tick{true};
	bool   pattern_break{false};
	bool   row_jump{false};
	byte   row_jump_target{};
	byte   repeat_row{};

	struct ChannelState {
		// efect curent
		Pattern::Cell cell;
		bool active{false};
		word memory['Z' + 1][4] = {};

		double sample_pos{};

		double period{}, target_period{};
		word volume{};
		byte panning{};

		const Sample *sample = nullptr;
		sbyte   loop_dir = +1;
	};
	vector<ChannelState> chst;
public:
	void load(const Module &md);
	static void effmem_handle(ChannelState&, int, byte&, byte&, byte&);

	void play(double rate = 48000);
	void render(sword*, size_t, double rate = 48000);
	void advance();
	void process_cell_tick0_init(const Pattern::Cell&, ChannelState&, bool portamento = false);
	void process_cell_tick0_eff(const Pattern::Cell&, ChannelState&, int, byte, byte, byte);
	void process_cell_rest_eff(const Pattern::Cell&, ChannelState&, int, byte, byte, byte);
};

}

#endif // AWEXM_PLAYER_HPP
