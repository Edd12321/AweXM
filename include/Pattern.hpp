#ifndef AWEXM_PATTERN_HPP
#define AWEXM_PATTERN_HPP
#include "Bin.hpp"
#include "util.hpp"

namespace AweXM {

class Pattern {
public:
	struct Cell {
		byte note{};
		byte instrument{};
		byte volume_column_byte{};
		byte effect_type{};
		byte effect_parameter{};
	};
	vector<vector<Cell>> table;
private:
	Pattern(const BinXMHeader&, const BinPattern&);
public:
};

}

#endif // AWEXM_PATTERN_HPP
