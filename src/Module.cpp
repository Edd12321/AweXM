#include "Module.hpp"
#include "Bin.hpp"
#include "util.hpp"

namespace AweXM {

Module::Module(const BinModule &m) : h(m.xm_header) {
	// ...
}

}
