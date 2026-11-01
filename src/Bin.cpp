#include "Bin.hpp"

namespace AweXM {

void read_bin_xm_header(istream &in, BinXMHeader &h) {
	binread(in, h.id_text);
	binread(in, h.module_name);
	binread(in, h.stripped_flag);
	binread(in, h.tracker_name);
	binread(in, h.version_number);
	if (h.version_number != 0x0104)
		throw std::runtime_error("unsupported XM format version "
		    + std::to_string(h.version_number));
	binread(in, h.header_size);
	binread(in, h.song_length);
	size_t pt_len;
	switch (h.stripped_flag) {
	case 0x1A:
		pt_len = 256;
		break;
	case 0x00:
		pt_len = h.song_length;
		break;
	default:
		throw std::runtime_error("invalid 38th byte "
		    + std::to_string(h.stripped_flag));
	}
	if (h.song_length < 1 || h.song_length > 256)
		throw std::runtime_error("invalid song length "
		    + std::to_string(h.song_length));
	binread(in, h.restart_position);
	binread(in, h.number_of_channels);
	binread(in, h.number_of_patterns);
	binread(in, h.number_of_instruments);
	binread(in, h.flags);
	binread(in, h.default_tempo);
	binread(in, h.default_bpm);
	h.pattern_order_table.resize(pt_len);
	binread(in, h.pattern_order_table);
}

void read_bin_pattern(istream &in, BinPattern &h) {
	binread(in, h.pattern_header_length);
	binread(in, h.packing_byte);
	binread(in, h.number_of_rows_in_pattern);
	binread(in, h.packed_pattern_data_size);
	h.packed_pattern_data.resize(h.packed_pattern_data_size);
	binread(in, h.packed_pattern_data);
}

void read_bin_instrument_header(istream &in, BinInstrumentHeader &h) {
	binread(in, h.instrument_size);
	binread(in, h.instrument_name);
	binread(in, h.instrument_type);
	binread(in, h.number_of_samples);
	if (h.number_of_samples) {
		binread(in, h.sample_header_size);
		binread(in, h.sample_keymap_assignments);
		binread(in, h.points_for_volume_envelope);
		binread(in, h.points_for_panning_envelope);
		binread(in, h.number_of_volume_points);
		binread(in, h.number_of_panning_points);
		binread(in, h.volume_sustain_point);
		binread(in, h.volume_loop_start_point);
		binread(in, h.volume_loop_end_point);
		binread(in, h.panning_sustain_point);
		binread(in, h.panning_loop_start_point);
		binread(in, h.panning_loop_end_point);
		binread(in, h.volume_type);
		binread(in, h.panning_type);
		binread(in, h.vibrato_type);
		binread(in, h.vibrato_sweep);
		binread(in, h.vibrato_depth);
		binread(in, h.vibrato_rate);
		binread(in, h.volume_fadeout);
		binread(in, h.reserved);
	} else if (h.instrument_size > 29) {
		h.garbage.resize(h.instrument_size - 29);
		binread(in, h.garbage);
	}
}

void read_bin_sample_header(istream &in, BinSampleHeader &h) {
	binread(in, h.sample_length);
	binread(in, h.sample_loop_start);
	binread(in, h.sample_loop_length);
	binread(in, h.volume);
	binread(in, h.finetune);
	binread(in, h.type);
	binread(in, h.panning);
	binread(in, h.relative_note_number);
	binread(in, h.delta_or_adpcm);
	//if (h.delta_or_adpcm != 0x00 && h.delta_or_adpcm != 0xAD)
	//	throw std::runtime_error("bad delta_or_adpcm "
	//	    + std::to_string(h.delta_or_adpcm));
	binread(in, h.sample_name);
}

void read_bin_instrument(istream &in, BinInstrument &i) {
	read_bin_instrument_header(in, i.instrument_header);
	auto ns = i.instrument_header.number_of_samples;
	if (ns > 0) {
		i.sample_headers.resize(ns);
		i.sample_datas.resize(ns);
		for (auto &it : i.sample_headers)
			read_bin_sample_header(in, it);
		for (size_t j = 0; j < ns; ++j) {
			const auto &h = i.sample_headers[j];
			auto len = h.sample_length;

			if (h.delta_or_adpcm == 0xAD && !(h.type & (1 << 4)) && len)
				len = 16 + ((len + 1) >> 1);

			i.sample_datas[j].resize(len);
			binread(in, i.sample_datas[j]);
		}
	}
}

void read_bin_module(istream &in, BinModule &m) {
	read_bin_xm_header(in, m.xm_header);
	m.patterns.resize(m.xm_header.number_of_patterns);
	m.instruments.resize(m.xm_header.number_of_instruments);
	for (auto &it : m.patterns)
		read_bin_pattern(in, it);
	for (auto &it : m.instruments)
		read_bin_instrument(in, it);
}

}
