#ifndef UTIL_HPP
#define UTIL_HPP
#include <array>
#include <cstddef>
#include <istream>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <vector>

template<typename T>
struct has_data {
	static constexpr bool value = false;
};
template<typename T>
struct has_data<std::vector<T>> {
	static constexpr bool value = true;
};
template<typename T, std::size_t n>
struct has_data<std::array<T, n>> {
	static constexpr bool value = true;
};

template<typename T, typename = typename std::enable_if<!has_data<T>::value>::type>
void binread(std::istream &in, T &dst, std::streamsize len = sizeof(T)) {
	in.read(reinterpret_cast<char*>(&dst), len);
	if (in.gcount() != len)
		throw std::runtime_error("couldn't read " + std::to_string(len) + " bytes");
}
template<typename T, typename = typename std::enable_if<has_data<T>::value>::type>
void binread(std::istream &in, T &dst) {
	if (dst.empty())
		return;
	binread(in, *dst.data(), dst.size() * sizeof(typename T::value_type));
}

#endif // UTIL_HPP
