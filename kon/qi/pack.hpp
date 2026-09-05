// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef PACK_18471EA5_EC9B_4807_9CE6_3F6C9FD9DD4F
#define PACK_18471EA5_EC9B_4807_9CE6_3F6C9FD9DD4F
#include <cstddef>

#define KON_QI_FFWD1(_arg_) static_cast<decltype(_arg_) &&>(_arg_)

namespace kon {
//
namespace qi {
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
    constexpr black_hole(auto &&...) noexcept {
    }
};

template <std::size_t N>
constexpr auto value_pack_element = []<auto... Ns>(index_sequence<Ns...>) -> decltype(auto) {
    return [](black_hole<Ns> &&..., auto &&nth, auto &&...) -> decltype(auto) {
        return KON_QI_FFWD1(nth);
    };
}(make_index_sequence<N>{});

template <auto... Vs>
struct value_pack {
    static consteval std::size_t size() noexcept {
        return sizeof...(Vs);
    }

    template <std::size_t I>
    static consteval auto get() noexcept {
        return value_pack_element<I>(Vs...);
    }

    static consteval auto visit(auto &&fun) noexcept {
        static_cast<decltype(fun) &&>(fun).template operator()<Vs...>();
    }
};
} // namespace qi
} // namespace kon
#endif // pack.hpp