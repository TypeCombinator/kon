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
    while (x != 0) [[likely]] {
        unsigned char i = countr_zero<T, true>(x);
        f(i);
        x &= (~(one << i));
    }
}

template <typename T, typename F>
constexpr void bit_reverse_for_each(T x, F &&f) {
    constexpr T mask = ~(static_cast<T>(1) << (sizeof(T) * 8 - 1));
    while (x != 0) [[likely]] {
        unsigned char i = countl_zero<T, true>(x);
        f(i);
        x &= (mask >> i);
    }
}

template <typename T>
class bit_iterator {
   public:
    bit_iterator(T x) noexcept
        : m_x{x} {
    }

    template <typename IT>
    bool next(IT &index) noexcept {
        constexpr T one = 1;
        if (m_x == 0) [[unlikely]] {
            return false;
        }
        index = countr_zero<T, true>(m_x);
        m_x &= (~(one << index));
        return true;
    }
   private:
    T m_x;
};

template <typename T>
class bit_reverse_iterator {
   public:
    bit_reverse_iterator(T x) noexcept
        : m_x{x} {
    }

    template <typename IT>
    bool next(IT &index) noexcept {
        constexpr T mask = ~(static_cast<T>(1) << (sizeof(T) * 8 - 1));
        if (m_x == 0) [[unlikely]] {
            return false;
        }
        index = countl_zero<T, true>(m_x);
        m_x &= (mask >> index);
        return true;
    }
   private:
    T m_x;
};

} // namespace kon

#endif // bitset.hpp