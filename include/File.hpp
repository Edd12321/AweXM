#ifndef AWEXM_FILEHEADER_HPP
#define AWEXM_FILEHEADER_HPP
#include <array>
#include <cstdint>
#include <istream>
#include <ostream>
#include <string>
#include <vector>
#include "Pattern.hpp"
#include "Instrument.hpp"

namespace AweXM {

class File;

class FileHeader {
private:
	using char17  = std::array<char, 17>;
	using char20  = std::array<char, 20>;
	using byte256 = std::array<std::uint8_t, 256>;
#define AWEXM_FILEHEADER_ENTRIES            \
	X(char17,        id_text)               \
	X(char20,        module_name)           \
	X(std::uint8_t,  always_1a)             \
	X(char20,        tracker_name)          \
	X(std::uint16_t, version_number)        \
	X(std::uint32_t, header_size)           \
	X(std::uint16_t, song_length)           \
	X(std::uint16_t, song_restart_position) \
	X(std::uint16_t, number_of_channels)    \
	X(std::uint16_t, number_of_patterns)    \
	X(std::uint16_t, number_of_instruments) \
	X(std::uint16_t, flags_field)           \
	X(std::uint16_t, default_tempo)         \
	X(std::uint16_t, default_bpm)           \
	X(byte256,       pattern_order_table)
#define X(T, name) T name{};
	AWEXM_FILEHEADER_ENTRIES
#undef X
public:
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
	friend std::istream &operator>>(std::istream&, File&);
	friend std::ostream &operator<<(std::ostream&, const File&);
};

}

#endif // AWEXM_FILEHEADER_HPP
