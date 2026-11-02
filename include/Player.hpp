#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Module.hpp"
#include "util.hpp"

namespace AweXM {

class Player {
private:
	const Module *m = nullptr;

	size_t tick{};
	word   bpm{};
	word   tempo{};
	word   row{};
	word   order_idx{};

	struct ChannelState {
		word instrument_idx{};
		word sample_idx{};
		dword sample_pos{};
		dword freq{};

		bool active = false;
		byte note{};
		// efecte (tbd)
	};
	vector<ChannelState> chst;
public:
	void load(const Module &md) {
		m = &md;
		bpm = md.get_default_bpm();
		tempo = md.get_default_tempo();
		tick = row = order_idx = 0;
		chst.resize(md.get_number_of_channels());
	}
	void advance();
};

}

#endif // PLAYER_HPP
