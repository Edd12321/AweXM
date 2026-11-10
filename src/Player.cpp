#include "Pattern.hpp"
#include "Player.hpp"
#include "util.hpp"

namespace AweXM {

void Player::load(const Module &md) {
	*this = Player();
	m = &md;
	bpm = md.get_default_bpm();
	tempo = md.get_default_tempo();
	chst.clear();
	chst.resize(md.get_number_of_channels());
}

void Player::effmem_handle(ChannelState &out, int E, byte &xx, byte &x, byte &y) {
	constexpr char M[] = {'1', '2', '3', '9', 'A', 'H', 'P'}; // verificam xx      (byte)
	constexpr char N[] = {'4', '7', 'R'};                     // verificam x sau y (nibble)
	
	for (char c : M) {
		if (E == c) {
			if (xx) {
				out.memory[E][0] = x;
				out.memory[E][1] = y;
				out.memory[E][2] = xx;
			} else {
				x  = out.memory[E][0];
				y  = out.memory[E][1];
				xx = out.memory[E][2];
			}
		}
	}
	for (char c : N) {
		if (E == c) {
			if (x)      out.memory[E][0] = x;
			else   x  = out.memory[E][0];

			if (y)      out.memory[E][1] = y;
			else   y  = out.memory[E][1];

			if (x || y) out.memory[E][2] = xx;
			else   xx = out.memory[E][2]; 
		}
	}
}

void Player::process_cell_tick0_init(const Pattern::Cell &cell, ChannelState &out, bool portamento) {
	if (cell.instrument)
		out.cell.instrument = cell.instrument;
	if (cell.note == 97)
		out.active = false;
	if (1 <= cell.note && cell.note <= 96) {
		out.cell.note = cell.note;

		if (portamento)
			return;

		out.sample_pos = 0;
		out.loop_dir = +1;
		out.sample = nullptr;

		const auto &I = m->get_instruments();
		if (out.cell.instrument && out.cell.instrument <= I.size()) {
			const auto &S = I[out.cell.instrument - 1].get_samples();
			auto s_idx = I[out.cell.instrument - 1].get_sample_keymap_assignments()[cell.note - 1];

			if (s_idx >= S.size())
				s_idx = 0;

			out.active = true;
			out.sample = &S[s_idx];
			out.volume = out.sample->get_volume();
			out.panning = out.sample->get_panning();
			out.period = note2period(cell.note - 1 + out.sample->get_relative_note_number(),
					out.sample->get_finetune(),
					m->get_linear());
		}
	}
}

void Player::process_cell_tick0_eff(const Pattern::Cell &cell, ChannelState &out, int E, byte xx, byte x, byte y) {
	if (!cell.effect_type && !xx)
		return;

	switch (E) {
	// cele care deja sunt facute in process_cell_rest
	case '0': [[fallthrough]];
	case 'A': [[fallthrough]];
	case '1': [[fallthrough]];
	case '2': [[fallthrough]];
	case '6':
		break;

	case '3':
		/* Tone portamento (#1) */ {
			if (cell.note < 1 || cell.note > 96 || !out.sample)
				break;
			out.target_period = note2period(
				cell.note - 1 + out.sample->get_relative_note_number(),
				out.sample->get_finetune(),
				m->get_linear());
		}
		break;
	case '8':
		/* Set panning */ {
			out.panning = xx;
		}
		break;
	case '9':
		/* Sample offset */ {
			out.sample_pos = 256 * xx;
		}
		break;
	case 'B':
		/* Position jump */ {
			row_jump = true;
			new_song_pos = xx;
			row_jump_target = 0;
		}
		break;
	case 'C':
		/* Set volume */ {
			out.volume = xx < 64 ? xx : 64;
		}
		break;
	case 'D':
		/* Pattern break */ {
			pattern_break = true;
			row_jump_target = x * 10 + y;
		}
		break;
	case 'E':
		switch (x) {
		case 1:
			/* Fine portamento up */ {
				out.period -= 4 * y;
			}
			break;
		case 2:
			/* Fine portamento down */ {
				out.period += 4 * y;
			}
			break;
		case 0xA:
			/* Fine volume slide up */ {
				out.volume = std::min(64, out.volume + y);
			}
			break;
		case 0xB:
			/* Fine volume slide down */ {
				out.volume = std::max(0, out.volume - y);
			}
			break;
		case 0xE:
			/* Pattern delay */ {
				repeat_row = y;
			}
			break;
		default:
			std::cerr << "Unimplemented E" << byte2hexstr(x)[1] << '\n';
			break;
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
	case 'G':
		global_volume = xx < 64 ? xx : 64;
		break;
	case 'K':
		if (xx == tick)
			out.active = false;
		break;
	default:
		std::cerr << "Unknown effect " << static_cast<char>(E) << '\n';
		break;
	}
}

void Player::process_cell_rest_eff(const Pattern::Cell &cell, ChannelState &out, int E, byte xx, byte x, byte y) {
	if (!cell.effect_type && !xx)
		return;
	switch (E) {
	// cele care deja sunt facute in process_cell_tick0_eff
	case '8': [[fallthrough]];
	case '9': [[fallthrough]];
	case 'B': [[fallthrough]];
	case 'C': [[fallthrough]];
	case 'D': [[fallthrough]];
	case 'E': [[fallthrough]];
	case 'F': [[fallthrough]];
	case 'G':
		break;

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
			out.period = note2period(
				note + out.sample->get_relative_note_number(),
				out.sample->get_finetune(),
				m->get_linear());
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
	case '3':
		/* Tone portamento (#2) */ {
			if (out.period > out.target_period)
				out.period = std::max(out.target_period, out.period - 4 * xx);
			else if (out.period < out.target_period)
				out.period = std::min(out.target_period, out.period + 4 * xx);
		}
		break;
	case '5':
		/* Volume slide with tone portamento */ {
			effmem_handle(out, 'A', xx, x, y);
			process_cell_rest_eff(cell, out, 'A', xx, x, y);
			process_cell_rest_eff(cell, out, '3', 00, 0, 0);
		}
		break;
	case '6':
		/* Volume slide with vibrato */ {
			effmem_handle(out, 'A', xx, x, y);
			process_cell_rest_eff(cell, out, 'A', xx, x, y);
			process_cell_rest_eff(cell, out, '4', 00, 0, 0);
		}
		break;
	case 'A':
		/* Volume slide */ {
			int v = out.volume;
			if (x)
				v += x;
			else v -= y;
			out.volume = std::min(std::max(0, v), 64);
		}
		break;
	case 'P':
		/* Panning slide */ {
			int p = out.panning;
			if (x)
				p += x;
			else p -= y;
			out.panning = std::max(0, p);
		}
		break;
	case 'K':
		if (x == tick)
			out.active = false;
		break;
	default:
		std::cerr << "Unknown effect " << static_cast<char>(E) <<'\n';
		break;
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
			if (ch.sample->get_data().empty()) {
				ch.active = false;
				continue;
			}
			auto sample_end = ch.sample->get_sample_loop_start() + ch.sample->get_sample_loop_length();
			auto sample_pos = static_cast<size_t>(std::max(0., std::floor(ch.sample_pos)));
			sample_pos = std::min(sample_pos, ch.sample->get_data().size() - 1);


			// mixare
			L += ch.sample->get_data()[sample_pos]
				* (ch.volume / 64.) * (global_volume / 64.) * (1. - ch.panning / 255.);
			R += ch.sample->get_data()[sample_pos]
				* (ch.volume / 64.) * (global_volume / 64.) * (ch.panning / 255.);

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
		if (first_tick) {
			state.cell = T[row][i];
			state.cell.effect_type = T[row][i].effect_type;
			state.cell.effect_parameter = T[row][i].effect_parameter;
		}

		if (first_tick)
			process_cell_tick0_init(state.cell, state,
			   (state.cell.volume_column_byte & 0xF0) == 0xF0
			|| (state.cell.effect_type == 3)
			|| (state.cell.effect_type == 5));

		//
		// efecte #1 (vol)
		//
		byte V = state.cell.volume_column_byte, Vx = V & 0x0F;
		switch (V & 0xF0) {
		case 0x10: [[fallthrough]];
		case 0x20: [[fallthrough]];
		case 0x30: [[fallthrough]];
		case 0x40: [[fallthrough]];
		case 0x50:
			/* (xx) Set volume */ {
				if (first_tick)
					process_cell_tick0_eff(state.cell, state, 'C', Vx - 0x10, 0, 0);
			}
			break;
		case 0x60:
			/* (-x) Volume slide down */ {
				if (!first_tick)
					process_cell_rest_eff(state.cell, state, 'A', 0, 0, Vx);
			}
			break;
		case 0x70:
			/* (+x) Volume slide up */ {
				if (!first_tick)
					process_cell_rest_eff(state.cell, state, 'A', 0, Vx, 0);
			}
			break;
		case 0x80:
			/* (Dx) Fine volume slide down */ {
				if (first_tick)
					process_cell_tick0_eff(state.cell, state, 'E', 0, 0xB, Vx);
			}
			break;
		case 0x90:
			/* (Ux) Fine volume slide up */ {
				if (first_tick)
					process_cell_tick0_eff(state.cell, state, 'E', 0, 0xA, Vx);
			}
			break;
		case 0xA0:
			/* (Sx) Set vibrato speed */
			std::cerr << "Unimplemented Sx\n";
			break;
		case 0xB0:
			/* (Vx) Vibrato */
			std::cerr << "Unimplemented Vx\n";
			break;
		case 0xC0:
			/* (Px) Set panning position */ {
				if (first_tick)
					process_cell_tick0_eff(state.cell, state, '8', Vx * 10, 0, 0);
			}
			break;
		case 0xD0:
			/* (Lx) Panning slide left */ {
				if (!first_tick)
					process_cell_rest_eff(state.cell, state, 'P', 0, 0, Vx);
			}
			break;
		case 0xE0:
			/* (Rx) Panning slide right */ {
				if (!first_tick)
					process_cell_rest_eff(state.cell, state, 'P', 0, Vx, 0);
			}
			break;
		case 0xF0:
			/* (Mx) Tone portamento */ {
				byte xx = Vx * 0x10, x = xx >> 4, y = xx & 0x0F;
				effmem_handle(state, '3', xx, x, y);
				if (first_tick)
					process_cell_tick0_eff(state.cell, state, '3', xx, x, y);
				else process_cell_rest_eff(state.cell, state, '3', xx, x, y);
			}
			break;
		}

		//
		// efecte #2 (normale)
		//
		char E = 0;
		if (state.cell.effect_type <= 0x09)
			E = static_cast<char>(state.cell.effect_type + '0');
		else if (state.cell.effect_type <= 0x21)
			E = static_cast<char>(state.cell.effect_type - 0xA + 'A');
		else std::cerr << "warning: bad effect " << state.cell.effect_type << '\n';
		byte xx = state.cell.effect_parameter;
		byte x = xx >> 4, y = xx & 0x0F;
		effmem_handle(state, E, xx, x, y);
		if (first_tick)
			process_cell_tick0_eff(state.cell, state, E, xx, x, y);
		else process_cell_rest_eff(state.cell, state, E, xx, x, y);
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
