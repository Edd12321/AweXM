#include "Player.hpp"
#include "util.hpp"

namespace AweXM {

void Player::advance() {
	if (!m)
		return;

	const auto &pot = m->get_pattern_order_table();
	const auto &T = m->get_patterns()[pot[order_idx]].get_table();

	for (size_t i = 0; i < chst.size(); ++i) {
		auto &state = chst[i];
		const auto &cell = T[row][i];

		if (tick == 0) {
			if (cell.instrument)
				state.instrument_idx = cell.instrument;

			if (cell.note) {
				if (cell.note == 97)
					state.active = false;
				if (1 <= cell.note && cell.note <= 96) {
					state.active     = true;
					state.note       = cell.note;
					state.sample_idx = 0;
				}
			}

			// ???
			// de gandit
		}
	}

	if (++tick >= tempo) {
		tick = 0;
		if (++row == T.size()) {
			row = 0;
			if (++order_idx >= m->get_song_length())
				order_idx = m->get_restart_position();
		}
	}
}

}
