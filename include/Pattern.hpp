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
private:
	vector<vector<Cell>> table;
public:
	Pattern() = default;
	Pattern(const BinXMHeader&, const BinPattern&);

	[[nodiscard]] const vector<vector<Cell>> &get_table() const noexcept {
		return table;
	}
	
	friend ostream &operator<<(ostream&, const Pattern&);
};

}

#endif // AWEXM_PATTERN_HPP
