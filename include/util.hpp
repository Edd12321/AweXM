#ifndef UTIL_HPP
#define UTIL_HPP
#include <array>
#include <cstddef>
#include <istream>

template<typename T>
void binread(std::istream &in, T &dst, std::streamsize len = sizeof(T)) {
	in.read(reinterpret_cast<char*>(&dst), len);
	if (in.gcount() != len)
		throw std::runtime_error("couldn't read " + std::to_string(len)
		                                          + " bytes");
}
template<typename T, std::size_t n>
void binread(std::istream &in, std::array<T, n> &dst) {
	binread(in, dst.data(), dst.size() * sizeof(T));
}

#endif // UTIL_HPP
