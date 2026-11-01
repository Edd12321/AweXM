#ifndef AWEXM_BIN_HPP
#define AWEXM_BIN_HPP
#include "util.hpp"

namespace AweXM {

struct BinXMHeader {
	array<char, 17> id_text;
	array<char, 20> module_name;
	byte            stripped_flag;
	array<char, 20> tracker_name;
	word            version_number;
	dword           header_size;
	word            song_length;
	word            restart_position;
	word            number_of_channels;
 	word            number_of_patterns;
	word            number_of_instruments;
	word            flags;
	word            default_tempo;
	word            default_bpm;
	vector<byte>    pattern_order_table;
};

struct BinPattern {
	dword           pattern_header_length;
	byte            packing_byte;
	word            number_of_rows_in_pattern;
	word            packed_pattern_data_size;
	vector<byte>    packed_pattern_data;
};

struct BinInstrumentHeader {
	dword           instrument_size;
	array<char, 22> instrument_name;
	byte            instrument_type;
	word            number_of_samples;

	vector<byte>    garbage;

	dword           sample_header_size;
	array<byte, 96> sample_keymap_assignments;
	array<word, 24> points_for_volume_envelope;
	array<word, 24> points_for_panning_envelope;
	byte            number_of_volume_points;
	byte            number_of_panning_points;
	byte            volume_sustain_point;
	byte            volume_loop_start_point;
	byte            volume_loop_end_point;
	byte            panning_sustain_point;
	byte            panning_loop_start_point;
	byte            panning_loop_end_point;
	byte            volume_type;
	byte            panning_type;
	byte            vibrato_type;
	byte            vibrato_sweep;
	byte            vibrato_depth;
	byte            vibrato_rate;
	word            volume_fadeout;
	array<word, 11> reserved;
};

struct BinSampleHeader {
	dword           sample_length;
	dword           sample_loop_start;
	dword           sample_loop_length;
	byte            volume;
	sbyte           finetune;
	byte            type;
	byte            panning;
	sbyte           relative_note_number;
	byte            delta_or_adpcm;
	array<char, 22> sample_name;
};

struct BinInstrument {
	BinInstrumentHeader     instrument_header;
	vector<BinSampleHeader> sample_headers;
	vector<vector<byte>>    sample_datas;
};

struct BinModule {
	BinXMHeader             xm_header;
	vector<BinPattern>      patterns;
	vector<BinInstrument>   instruments;
};

void read_bin_xm_header(istream&, BinXMHeader&);
void read_bin_pattern(istream&, BinPattern&);
void read_bin_instrument_header(istream&, BinInstrumentHeader&);
void read_bin_sample_header(istream&, BinSampleHeader&);
void read_bin_instrument(istream&, BinInstrument&);
void read_bin_module(istream&, BinModule&);

}

#endif // AWEXM_BIN_HPP
