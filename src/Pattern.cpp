#include "Bin.hpp"
#include "Pattern.hpp"
#include "util.hpp"

namespace AweXM {

Pattern::Pattern(const BinXMHeader &h, const BinPattern &p) {
	size_t k = 0;
	auto nr = p.number_of_rows_in_pattern, nc = h.number_of_channels;
	
	auto getbyte = [&]() {
		if (k >= p.packed_pattern_data_size)
			throw std::runtime_error("malformed packed patterndata");
		return p.packed_pattern_data[k++];
	};

	table.resize(nr);
	for (size_t i = 0; i < nr; ++i) {
		table[i].resize(nc);
		for (size_t j = 0; j < nc; ++j) {
			byte dbyt = getbyte();
			auto &c = table[i][j];

			if (dbyt & 0x80) {
				if (dbyt & 0x01) c.note               = getbyte();
				if (dbyt & 0x02) c.instrument         = getbyte();
				if (dbyt & 0x04) c.volume_column_byte = getbyte();
				if (dbyt & 0x08) c.effect_type        = getbyte();
				if (dbyt & 0x10) c.effect_parameter   = getbyte();
			} else {
				c.note               = dbyt;
				c.instrument         = getbyte();
				c.volume_column_byte = getbyte();
				c.effect_type        = getbyte();
				c.effect_parameter   = getbyte();
			}
		}
	}
}

}
