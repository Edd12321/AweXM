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
		out.loop_dir = +1;

		const auto &I = m->get_instruments();
		if (out.cell.instrument && out.cell.instrument <= I.size()) {
			const auto &S = I[out.cell.instrument - 1].get_samples();
			auto s_idx = I[out.cell.instrument - 1].get_sample_keymap_assignments()[cell.note - 1];

			if (s_idx < S.size()) {
				out.sample = &S[s_idx];
				out.volume = out.sample->get_volume();
				out.panning = out.sample->get_panning();
				out.period = note2period(cell.note - 1 + out.sample->get_relative_note_number(),
						out.sample->get_finetune(),
						m->get_linear());
			}
		}
	}
	if (!cell.effect_type && !xx)
		return;
	out.cell.effect_type = cell.effect_type;
	out.cell.effect_parameter = xx;

	switch (E) {
	case '8':
		/* Set panning */ {
			out.panning = xx;
		}
		break;
	case '9':
		/* Sample offset */ {
			out.sample_pos = xx;
		}
		break;
	case 'B':
		/* Position jump */ {
			row_jump = true;
			new_song_pos = xx;
			row_jump_target = 0;
		}
		break;
	case 'D':
		/* Pattern break */ {
			pattern_break = true;
			row_jump_target = x * 10 + y;
		}
		break;
	case 'C':
		/* Set volume */ {
			out.volume = xx < 64 ? xx : 64;
		}
		break;
	case 'F':
		/* Set speed */ {
			if (xx == 0) {
				std::cerr << "invalid F arg " << xx << '\n';
				break;
			}
			if (xx <= 31)
				tempo = xx;
			else bpm = xx;
		}
		break;
	}
}

void Player::process_cell_rest(const Pattern::Cell &cell, ChannelState &out, char E, byte xx, byte x, byte y) {
	if (!cell.effect_type && !xx)
		return;
	switch (E) {
	case '0':
		/* Arpeggio */ {
			auto note = out.cell.note - 1;
			switch (tick % 3) {
			case 0: break;
			case 1: note += x; break;
			case 2: note += y; break;
			}
			if (!out.sample)
				break;
			out.period = note2period(note, out.sample->get_finetune(), m->get_linear());
		}
		break;
	case '1':
		/* Portamento up */ {
			out.period -= 4 * xx;
		}
		break;
	case '2':
		/* Portamento down */ {
			out.period += 4 * xx;
		}
		break;
	case 'A':
		/* Volume slide */ {
			if (x)
				out.volume += x;
			else out.volume -= y;
		}
	}
}

void Player::play(double rate) {
	constexpr size_t cnt = 512;
	vector<sword> buf(2 * cnt);
	for (;;) {
		render(buf.data(), cnt, rate);
		binwrite(std::cout, buf);
		std::cout.flush();
	}
}

void Player::render(sword *out, size_t framecnt, double rate) {
	for (size_t i = 0; i < framecnt; ++i, --frames_til_tick) {
		// un row contine mai multe tickuri si un tick mai multe frameuri stereo
		// cat de multe frameuri stereo incap intr-un tick?
		// (ca sa stim pentru ce valori ale lui i apelam advance())
		//
		// raspuns:
		// un tick dureaza 2.5/bpm secunde
		// un frame dureaza 1/rate secunde (din definitie)
		// cautam un n a.i. n*1/rate = 2.5/bpm
		// => n = 2.5*rate/bpm
		if (frames_til_tick <= 0.) {
			advance();
			frames_til_tick += 2.5 * rate / bpm;
		}
		double L = 0, R = 0;
		for (auto &ch : chst) {
			if (!ch.active || !ch.sample)
				continue;
			auto sample_end = ch.sample->get_sample_loop_start() + ch.sample->get_sample_loop_length();

			// mixare
			L += ch.sample->get_data()[static_cast<size_t>(ch.sample_pos)]
				* (ch.volume / 64.) * (1. - ch.panning / 255.);
			R += ch.sample->get_data()[static_cast<size_t>(ch.sample_pos)]
				* (ch.volume / 64.) * (ch.panning / 255.);

			// avansare in sample in fct de viteza
			ch.sample_pos += ch.loop_dir * period2freq(ch.period, m->get_linear()) / rate;

			// looping
			switch (ch.sample->get_loop_type()) {
			case Sample::loop_type::FORWARD_LOOP:
				if (ch.sample_pos >= sample_end)
					ch.sample_pos = ch.sample->get_sample_loop_start();
				break;

			case Sample::loop_type::NO_LOOP:
				if (static_cast<size_t>(ch.sample_pos) >= ch.sample->get_data().size())
					ch.active = false;
				break;
			
			case Sample::loop_type::PING_PONG_LOOP:
				if (ch.loop_dir == -1 && ch.sample_pos <= ch.sample->get_sample_loop_start()) {
					ch.loop_dir *= -1;
					ch.sample_pos = ch.sample->get_sample_loop_start();
				}
				if (ch.sample_pos >= sample_end) {
					ch.loop_dir *= -1;
					ch.sample_pos = sample_end - 1;
				}
				break;
			}
		}
		out[2 * i] = static_cast<sword>(std::min(std::max(-32768L, std::lround(L)), 32767L));
		out[2 * i + 1] = static_cast<sword>(std::min(std::max(-32768L, std::lround(R)), 32767L));
	}
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

		char E = 0;
		if (state.cell.effect_type <= 0x09)
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
