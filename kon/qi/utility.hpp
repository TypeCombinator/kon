// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef UTILITY_44B17ECA_BB77_47E5_8054_AB3A5CA86A50
#define UTILITY_44B17ECA_BB77_47E5_8054_AB3A5CA86A50
#include <cstdint>

#define KON_FAST_FWD1(_arg_) static_cast<decltype(_arg_)&&>(_arg_)

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
    static constexpr std::size_t value = N;
};

template <auto...>
struct black_hole {
    constexpr black_hole(auto&&...) noexcept {
    }
};

template <std::size_t N>
constexpr auto value_pack_element = []<auto... Ns>(index_sequence<Ns...>) -> decltype(auto) {
    return [](black_hole<Ns>&&..., auto&& nth, auto&&...) -> decltype(auto) {
        return KON_FAST_FWD1(nth);
    };
}(make_index_sequence<N>{});

} // namespace kon

#endif // utility.hpp