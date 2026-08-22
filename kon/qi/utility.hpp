// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef UTILITY_44B17ECA_BB77_47E5_8054_AB3A5CA86A50
#define UTILITY_44B17ECA_BB77_47E5_8054_AB3A5CA86A50
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
} // namespace kon

#endif // utility.hpp