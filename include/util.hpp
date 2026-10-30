#ifndef AWEXM_UTIL_HPP
#define AWEXM_UTIL_HPP
#include <array>
#include <cstddef>
#include <istream>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace AweXM {

template<std::size_t n>
using buffer = std::array<std::uint8_t, n>;

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

#define AWEXM_DRY_AVOIDER_DEF(oper, stream, C, extra)                               \
template<typename T, typename = typename std::enable_if<!has_data<T>::value>::type> \
void bin##oper(std::stream &s, T &dst, std::streamsize len = sizeof(T)) {           \
    s.oper(reinterpret_cast<C>(&dst), len);                                         \
    extra;                                                                          \
}                                                                                   \
                                                                                    \
template<typename T, typename = typename std::enable_if<has_data<T>::value>::type>  \
void bin##oper(std::stream &s, T &dst) {                                            \
    if (dst.empty())                                                                \
        return;                                                                     \
    bin##oper(s, *dst.data(), dst.size() * sizeof(typename T::value_type));         \
}

AWEXM_DRY_AVOIDER_DEF(write, ostream, const char*,)
AWEXM_DRY_AVOIDER_DEF(read,  istream, char*,
	if (s.gcount() != len)
		throw std::runtime_error("couldn't read " + std::to_string(len) + " bytes");
)

}

#endif // AWEXM_UTIL_HPP
