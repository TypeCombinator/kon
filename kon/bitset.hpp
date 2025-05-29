// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef BITSET_18B88FF9_E29C_4D5F_84F4_9FE6B8573C24
#define BITSET_18B88FF9_E29C_4D5F_84F4_9FE6B8573C24
#include <kon/bit.hpp>

namespace kon {
// It's suitable for sparse data.
template <typename T, typename F>
constexpr void bit_for_each(T x, F &&f) {
    constexpr T one = 1;
    while (x != 0) {
        unsigned char i = countr_zero<T, false>(x);
        f(i);
        x &= (~(one << i));
        i = countr_zero<T, false>(x);
    }
}

} // namespace kon

#endif // bitset.hpp