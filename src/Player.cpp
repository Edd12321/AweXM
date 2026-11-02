#include "Pattern.hpp"
#include "Player.hpp"
#include "util.hpp"

namespace AweXM {

void Player::process_cell_tick0(const Pattern::Cell &cell, ChannelState &out, char E, byte xx, byte x, byte y) {
	if (cell.instrument)
		out.cell.instrument = cell.instrument;
	if (cell.note == 97)
		out.active = false;
	if (1 <= cell.note && cell.note <= 96) {
		out.active = true;
		out.cell.note = cell.note;
		out.sample_pos = 0;
	}

	// wip
	std::cerr << "DEBUG: " << E << ' ' << xx << ' ' << x << ' ' << y << '\n';
}

void Player::process_cell_rest(const Pattern::Cell &cell, ChannelState &out, char E, byte xx, byte x, byte y) {
	// wip placeholder
	std::cerr << "DEBUG: " << E << ' ' << xx << ' ' << x << ' ' << y << ' ' << cell.note << ' ' << out.cell.note << '\n';
}

void Player::advance() {
	if (!m)
		return;

	const auto &pot = m->get_pattern_order_table();
	const auto &T = m->get_patterns()[pot[song_pos]].get_table();

	for (size_t i = 0; i < chst.size(); ++i) {
		auto &state = chst[i];
		if (first_tick)
			state.cell = T[row][i];

		char E;
		if (state.cell.effect_type && state.cell.effect_type <= 0x09)
			E = static_cast<char>(state.cell.effect_type + '0');
		else if (state.cell.effect_type <= 0x21)
			E = static_cast<char>(state.cell.effect_type - 0xA + 'A');
		else std::cerr << "warning: bad effect " << state.cell.effect_type << '\n';
		auto xx = state.cell.effect_parameter;
		auto x = xx >> 4, y = xx & 0x0F;
		
		if (first_tick)
			process_cell_tick0(state.cell, state, E, xx, x, y);
		else process_cell_rest(state.cell, state, E, xx, x, y);
	}

	// from pollak.thebe.de/b/the-xm-format:
	first_tick = false;
	if (++tick >= tempo) {
		tick = 0;
		if (repeat_row > 0)
			--repeat_row;
		else {
			first_tick = true;
			if (++row >= T.size())
				pattern_break = true;
		}
		if (row_jump || pattern_break)
			row = row_jump_target;
		if (pattern_break) {
			song_pos = new_song_pos;
			if (song_pos >= m->get_song_length())
				song_pos = m->get_restart_position();
			new_song_pos = song_pos + 1;
			row_jump_target = 0;
		}
		row_jump = false;
		pattern_break = false;
	}
}

}
