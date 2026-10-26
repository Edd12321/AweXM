#ifndef XMFILE_HPP
#define XMFILE_HPP
#include <istream>
#include <ostream>
#include <string>
#include "XMFileHeader.hpp"
#include "XMPattern.hpp"
#include "XMInstrument.hpp"

class XMFile {
private:
	XMFileHeader xh;
	std::vector<XMPattern> v_xp;
	std::vector<XMInstrument> v_xi;
public:
	explicit XMFile(std::string const&);
	XMFile() = delete;
	XMFile& operator=(const XMFile&) = default;
	XMFile(const XMFile&) = default;
	~XMFile() = default;
	friend std::istream &operator>>(std::istream&, XMFile&);
	friend std::ostream &operator<<(std::ostream&, const XMFile&);
};

#endif // XMFILE_HPP
