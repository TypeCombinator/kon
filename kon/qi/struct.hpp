// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef STRUCT_432BBF93_B816_4A5C_963E_0EDA8E33212F
#define STRUCT_432BBF93_B816_4A5C_963E_0EDA8E33212F
#include <kon/qi/utility.hpp>

namespace kon {
//
namespace qi {
template <std::size_t N>
struct cure_all {
    template <class T>
    operator T() const noexcept;
};

template <class T, std::size_t Start = 0, std::size_t End = 256>
consteval std::size_t member_count() noexcept {
    constexpr std::size_t Middle = (Start + End) >> 1;
    constexpr int r = []<std::size_t... Ns>(kon::index_sequence<Ns...>) {
        if constexpr (not requires { T{cure_all<Ns>{}...}; }) {
            return 1;
        } else if constexpr (
            requires { T{cure_all<Ns>{}...}; }
            and not requires { T{cure_all<Ns>{}..., cure_all<Middle + 1>{}}; }) {
            return 0;
        } else {
            return -1;
        }
    }(kon::make_index_sequence<Middle>());

    if (r > 0) {
        return member_count<T, Start, Middle>();
    } else if (r < 0) {
        return member_count<T, Middle + 1, End>();
    } else {
        return Middle;
    }
}
} // namespace qi
} // namespace kon

#endif // struct.hpp