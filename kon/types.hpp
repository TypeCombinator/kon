// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef TYPES_F8436028_1D08_400B_90D5_B15ACC7C8432
#define TYPES_F8436028_1D08_400B_90D5_B15ACC7C8432

#include <cstddef>
#include <type_traits>
#include <utility>

#define KON_CARRAY_SIZE(_carray_) (sizeof(_carray_) / sizeof(_carray_[0]))

namespace kon {

template <typename T, std::size_t N>
constexpr size_t array_size(const T (&)[N]) noexcept {
    return N;
}

namespace detail {
template <auto MP>
struct member_type { };

template <typename T, typename MT, MT T::* MP>
struct member_type<MP> {
    using type = MT;
};
} // namespace detail

template <auto MP>
using member_t = detail::member_type<MP>::type;

// Reference: https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2023/p2593r1.html
template <typename...>
constexpr bool always_false_v = false;

} // namespace kon

#endif /* types.hpp */