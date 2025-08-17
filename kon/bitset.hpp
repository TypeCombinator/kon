// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef BITSET_18B88FF9_E29C_4D5F_84F4_9FE6B8573C24
#define BITSET_18B88FF9_E29C_4D5F_84F4_9FE6B8573C24
#include <kon/bit.hpp>
#include <cstdint>

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

enum class bitset_mask_t {
};
constexpr auto bitset_mask = bitset_mask_t{};

// Aggregate bitset
template <std::size_t N, typename T = std::uint32_t>
class bitset {
   public:
    static constexpr std::size_t storage_element_bit_width = sizeof(T) * 8;
    static constexpr std::size_t storage_size =
        (N + storage_element_bit_width - 1) / storage_element_bit_width;

    static constexpr std::size_t pos_to_x(std::size_t pos) noexcept {
        return pos % storage_element_bit_width;
    }

    static constexpr std::size_t pos_to_y(std::size_t pos) noexcept {
        return pos / storage_element_bit_width;
    }

    static constexpr auto last_mask = lsb_mask<T>(pos_to_x(N - 1));

    T data[storage_size];

    [[nodiscard]]
    constexpr std::size_t size() const noexcept {
        return N;
    }

    constexpr bitset &set() noexcept {
        for (std::size_t i = 0; i < storage_size; ++i) {
            data[i] = ~static_cast<T>(0);
        }
        return *this;
    }

    constexpr bitset &set(std::size_t pos) noexcept {
        data[pos_to_y(pos)] |= (static_cast<T>(1) << pos_to_x(pos));
        return *this;
    }

    constexpr bitset &set(std::size_t msb, std::size_t lsb) noexcept {
        auto ly = pos_to_y(lsb);
        auto lx = pos_to_x(lsb);

        auto my = pos_to_y(msb);
        auto mx = pos_to_x(msb);
        if (ly == my) {
            data[ly] |= bit_mask<T>(mx, lx);
            return *this;
        }
        data[ly] |= msb_mask<T>(lx);
        for (ly++; ly < my; ly++) {
            data[ly] = ~static_cast<T>(0);
        }
        data[my] |= lsb_mask<T>(mx);
        return *this;
    }

    constexpr bitset &reset() noexcept {
        for (std::size_t i = 0; i < storage_size; ++i) {
            data[i] = 0;
        }
        return *this;
    }

    constexpr bitset &reset(std::size_t pos) noexcept {
        data[pos_to_y(pos)] &= (~(static_cast<T>(1) << pos_to_x(pos)));
        return *this;
    }

    constexpr bitset &reset(std::size_t msb, std::size_t lsb) noexcept {
        auto ly = pos_to_y(lsb);
        auto lx = pos_to_x(lsb);

        auto my = pos_to_y(msb);
        auto mx = pos_to_x(msb);
        if (ly == my) {
            data[ly] &= bit_imask<T>(mx, lx);
            return *this;
        }
        data[ly] &= msb_imask<T>(lx);
        for (ly++; ly < my; ly++) {
            data[ly] = 0;
        }
        data[my] &= lsb_imask<T>(mx);
        return *this;
    }

    constexpr bitset &flip() noexcept {
        for (std::size_t i = 0; i < storage_size; ++i) {
            data[i] = ~data[i];
        }
        return *this;
    }

    constexpr bitset &flip(std::size_t pos) noexcept {
        data[pos_to_y(pos)] ^= (static_cast<T>(1) << pos_to_x(pos));
        return *this;
    }

    constexpr bitset &flip(std::size_t msb, std::size_t lsb) noexcept {
        auto ly = pos_to_y(lsb);
        auto lx = pos_to_x(lsb);

        auto my = pos_to_y(msb);
        auto mx = pos_to_x(msb);
        if (ly == my) {
            data[ly] ^= bit_mask<T>(mx, lx);
            return *this;
        }
        data[ly] ^= msb_mask<T>(lx);
        for (ly++; ly < my; ly++) {
            data[ly] ^= (~static_cast<T>(0));
        }
        data[my] ^= lsb_mask<T>(mx);
        return *this;
    }

    [[nodiscard]]
    constexpr bool all() const noexcept {
        std::size_t i = 0;
        for (; i < storage_size - 1; i++) {
            if ((~data[i]) == 0) {
                return false;
            }
        }
        return ((~data[i]) & last_mask) == 0;
    }

    [[nodiscard]]
    constexpr bool any() const noexcept {
        std::size_t i = 0;
        for (; i < storage_size - 1; i++) {
            if (data[i]) {
                return true;
            }
        }
        return data[i] & last_mask;
    }

    [[nodiscard]]
    constexpr bool none() const noexcept {
        std::size_t i = 0;
        for (; i < storage_size - 1; i++) {
            if (data[i]) {
                return false;
            }
        }
        return (data[i] & last_mask) == 0;
    }

    [[nodiscard]]
    constexpr std::size_t count() noexcept {
        std::size_t count = 0;
        std::size_t i = 0;
        for (; i < storage_size - 1; i++) {
            count += popcount(data[i]);
        }
        return count + popcount(data[i] & last_mask);
    }

    [[nodiscard]]
    constexpr bool operator[](std::size_t pos) const {
        return (data[pos_to_y(pos)] & (static_cast<T>(1) << pos_to_x(pos))) != 0;
    }

    [[nodiscard]]
    friend constexpr auto operator==(const bitset &lhs, const bitset &rhs) -> bool {
        std::size_t i = 0;
        for (; i < storage_size - 1; ++i) {
            if (lhs.data[i] != rhs.data[i]) {
                return false;
            }
        }
        return (lhs.data[i] & last_mask) == (rhs.data[i] & last_mask);
    }

    [[nodiscard]]
    friend constexpr auto operator!=(const bitset &lhs, const bitset &rhs) -> bool {
        for (std::size_t i = 0; i < storage_size; ++i) {
            if (lhs.data[i] != rhs.data[i]) {
                return true;
            }
        }
        return false;
    }
};

} // namespace kon

#endif // bitset.hpp