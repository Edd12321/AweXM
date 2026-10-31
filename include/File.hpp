#ifndef AWEXM_FILEHEADER_HPP
#define AWEXM_FILEHEADER_HPP
#include <cstdint>
#include <istream>
#include <ostream>
#include <string>
#include <vector>
#include "Pattern.hpp"
#include "Instrument.hpp"
#include "util.hpp"

namespace AweXM {

class File;

class FileHeader {
private:
#define AWEXM_FILEHEADER_ENTRIES            \
    X(buffer<17>,    id_text)               \
    X(buffer<20>,    module_name)           \
    X(std::uint8_t,  always_0x1a)           \
    X(buffer<20>,    tracker_name)          \
    X(std::uint16_t, version_number)        \
    X(std::uint32_t, header_size)           \
    X(std::uint16_t, song_length)           \
    X(std::uint16_t, song_restart_position) \
    X(std::uint16_t, number_of_channels)    \
    X(std::uint16_t, number_of_patterns)    \
    X(std::uint16_t, number_of_instruments) \
    X(std::uint16_t, flags_field)           \
    X(std::uint16_t, default_tempo)         \
    X(std::uint16_t, default_bpm)
#define X(T, name) T name{};
	AWEXM_FILEHEADER_ENTRIES
#undef X
	std::vector<std::uint8_t> pattern_order_table;
public:
#define X(T, name) const T &get_##name() const noexcept;
	AWEXM_FILEHEADER_ENTRIES
#undef X

	friend std::istream &operator>>(std::istream&, FileHeader&);
	friend std::istream &operator>>(std::istream&, File&);
	friend std::ostream &operator<<(std::ostream&, const FileHeader&);
};

class File {
private:
	FileHeader xh;
	std::vector<Pattern> v_xp;
	std::vector<Instrument> v_xi;
public:
	explicit File(const std::string&);
	File() = default;
	File &operator=(const File&);
	File(const File&);
	File &operator=(File&&) noexcept;
	File(File&&) noexcept;
	~File();

	[[nodiscard]] const FileHeader &get_file_header() const noexcept;
	[[nodiscard]] const std::vector<Pattern> &get_patterns() const noexcept;
	[[nodiscard]] const std::vector<Instrument> &get_instruments() const noexcept;

	friend std::istream &operator>>(std::istream&, File&);
	friend std::ostream &operator<<(std::ostream&, const File&);
};

}

#endif // AWEXM_FILEHEADER_HPP
