#ifndef AWEXM_MODULE_HPP
#define AWEXM_MODULE_HPP
#include "Bin.hpp"
#include "Instrument.hpp"
#include "Pattern.hpp"
#include "util.hpp"

namespace AweXM {

class Module {
private:
	BinXMHeader h;
	vector<Pattern> pats;
	vector<Instrument> instrs;
public:
	explicit Module(const BinModule&);
};

}

#endif // AWEXM_MODULE_HPP
