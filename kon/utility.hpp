// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef UTILITY_616CA32E_7000_4FDA_A6BB_0D093E8310D3
#define UTILITY_616CA32E_7000_4FDA_A6BB_0D093E8310D3
#include <cstdint>

namespace kon {
template <typename T, T...>
struct integer_sequence { };

template <std::size_t... Ns>
using index_sequence = integer_sequence<std::size_t, Ns...>;

template <std::size_t N>
using make_index_sequence =
#if defined(__clang__) || defined(_MSC_VER)
    __make_integer_seq<integer_sequence, std::size_t, N>;
#else
    index_sequence<__integer_pack(N)...>;
#endif

template <std::size_t N>
struct size_constant {
    static inline constexpr std::size_t value = N;
};

template <typename T, T... Ns>
    requires(sizeof...(Ns) > 0)
static constexpr T max() noexcept {
    T values[]{Ns...};
    const T *cur = values;

    T result = *cur++;
    for (const T *end = values + sizeof...(Ns); cur < end; cur++) {
        if (result < *cur) {
            result = *cur;
        }
    }
    return result;
}

template <std::uint64_t N>
static consteval auto narrow_uint() noexcept {
    if constexpr (N <= static_cast<uint8_t>(~std::uint8_t{})) {
        return std::uint8_t{};
    } else if constexpr (N <= static_cast<uint16_t>(~std::uint16_t{})) {
        return std::uint16_t{};
    } else if constexpr (N <= static_cast<uint32_t>(~std::uint32_t{})) {
        return std::uint32_t{};
    } else {
        return std::uint64_t{};
    }
}

template <std::size_t N>
using narrow_uint_t = decltype(narrow_uint<N>());
} // namespace kon

#define KON_FORWARD(_arg_) static_cast<decltype(_arg_) &&>(_arg_)


#endif // utility.hpp