#ifndef AWEXM_UTIL_HPP
#define AWEXM_UTIL_HPP
#include <array>
#include <cstddef>
#include <iostream>
#include <istream>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace AweXM {

using byte   = std::uint8_t;
using sbyte  = std::int8_t;
using word   = std::uint16_t;
using sword  = std::int16_t;
using dword  = std::uint32_t;
using sdword = std::int32_t;

using std::size_t;

using std::array;
using std::istream;
using std::ostream;
using std::string;
using std::vector;

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

#define AWEXM_DRY_AVOIDER_DEF(oper, stream, C, before, after)                       \
template<typename T, typename = typename std::enable_if<!has_data<T>::value>::type> \
void bin##oper(std::stream &s, T &dst, std::streamsize len = sizeof(T)) {           \
    before;                                                                         \
    s.oper(reinterpret_cast<C>(&dst), len);                                         \
    after;                                                                          \
}                                                                                   \
                                                                                    \
template<typename T, typename = typename std::enable_if<has_data<T>::value>::type>  \
void bin##oper(std::stream &s, T &dst) {                                            \
    if (dst.empty())                                                                \
        return;                                                                     \
    bin##oper(s, *dst.data(), dst.size() * sizeof(typename T::value_type));         \
}

AWEXM_DRY_AVOIDER_DEF(write, ostream, const char*
		,
		,
)
AWEXM_DRY_AVOIDER_DEF(read,  istream, char*
	, // inainte de s.oper(...)
	auto tell = s.tellg();

	, // dupa
	if (s.gcount() != len)
		throw std::runtime_error("couldn't read " + std::to_string(len) + " bytes"
		                       + " at offset " + std::to_string(tell));
)

}

#endif // AWEXM_UTIL_HPP
