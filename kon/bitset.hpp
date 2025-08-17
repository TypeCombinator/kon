// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef BITSET_18B88FF9_E29C_4D5F_84F4_9FE6B8573C24
#define BITSET_18B88FF9_E29C_4D5F_84F4_9FE6B8573C24
#include <kon/bit.hpp>
#include <cstdint>

namespace kon {

// Aggregate bitset
template <std::size_t N, typename T = std::uint32_t>
class bitset {
   public:
    static constexpr std::size_t element_bit_width = sizeof(T) * 8;
    static constexpr std::size_t element_number = (N + element_bit_width - 1) / element_bit_width;

    static constexpr std::size_t pos_to_x(std::size_t pos) noexcept {
        return pos % element_bit_width;
    }

    static constexpr std::size_t pos_to_y(std::size_t pos) noexcept {
        return pos / element_bit_width;
    }

    static constexpr auto last_mask = kon::lsb_mask<T>(pos_to_x(N - 1));

    T data[element_number];

    [[nodiscard]]
    static constexpr std::size_t size() noexcept {
        return N;
    }

    [[nodiscard]]
    static constexpr std::size_t capacity() noexcept {
        return element_number * element_bit_width;
    }

    constexpr bitset &set() noexcept {
        for (std::size_t i = 0; i < element_number; i++) {
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
        const auto lx = pos_to_x(lsb);

        const auto my = pos_to_y(msb);
        const auto mx = pos_to_x(msb);
        if (ly == my) {
            data[ly] |= kon::bit_mask<T>(mx, lx);
            return *this;
        }
        data[ly] |= kon::msb_mask<T>(lx);
        for (ly++; ly < my; ly++) {
            data[ly] = ~static_cast<T>(0);
        }
        data[my] |= kon::lsb_mask<T>(mx);
        return *this;
    }

    constexpr bitset &reset() noexcept {
        for (std::size_t i = 0; i < element_number; i++) {
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
        for (std::size_t i = 0; i < element_number; i++) {
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
        const auto lx = pos_to_x(lsb);

        const auto my = pos_to_y(msb);
        const auto mx = pos_to_x(msb);
        if (ly == my) {
            data[ly] ^= kon::bit_mask<T>(mx, lx);
            return *this;
        }
        data[ly] ^= kon::msb_mask<T>(lx);
        for (ly++; ly < my; ly++) {
            data[ly] ^= (~static_cast<T>(0));
        }
        data[my] ^= kon::lsb_mask<T>(mx);
        return *this;
    }

    [[nodiscard]]
    constexpr bool all() const noexcept {
        std::size_t i = 0;
        for (; i < (element_number - 1); i++) {
            if ((~data[i]) == 0) {
                return false;
            }
        }
        return ((~data[i]) & last_mask) == 0;
    }

    [[nodiscard]]
    constexpr bool any() const noexcept {
        std::size_t i = 0;
        for (; i < (element_number - 1); i++) {
            if (data[i]) {
                return true;
            }
        }
        return data[i] & last_mask;
    }

    [[nodiscard]]
    constexpr bool none() const noexcept {
        std::size_t i = 0;
        for (; i < (element_number - 1); i++) {
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
        for (; i < (element_number - 1); i++) {
            count += kon::popcount(data[i]);
        }
        return count + kon::popcount(data[i] & last_mask);
    }

    [[nodiscard]]
    constexpr std::size_t countr_zero() const noexcept {
        std::size_t count = 0;
        std::size_t i = 0;
        for (; i < (element_number - 1); i++) {
            if (data[i] == 0) {
                count += element_bit_width;
            } else {
                return count + kon::countr_zero<T, true>(data[i]);
            }
        }
        constexpr auto nx = pos_to_x(N);
        if constexpr (nx == 0) { // Aligned?
            return count + kon::countr_zero(data[i]);
        } else {
            return count + kon::countr_zero<T, true>(data[i] | kon::msb_mask<T>(nx));
        }
    }

    [[nodiscard]]
    constexpr std::size_t countl_zero() const noexcept {
        constexpr auto nx = pos_to_x(N);

        std::size_t count;
        T e = data[element_number - 1];
        if constexpr (nx == 0) {
            if (e != 0) {
                return kon::countl_zero<T, true>(e);
            }
            count = element_bit_width;
        } else {
            e &= kon::msb_imask<T>(nx);
            if (e != 0) {
                return kon::countl_zero<T, true>(e) - (element_bit_width - nx);
            }
            count = nx;
        }
        for (auto i = element_number - 1; i > 0; i--) {
            e = data[i - 1];
            if (e == 0) {
                count += element_bit_width;
            } else {
                return count + kon::countl_zero<T, true>(e);
            }
        }
        return count;
    }

    [[nodiscard]]
    constexpr bool operator[](std::size_t pos) const noexcept {
        return (data[pos_to_y(pos)] & (static_cast<T>(1) << pos_to_x(pos))) != 0;
    }

    [[nodiscard]]
    friend constexpr bool operator==(const bitset &lhs, const bitset &rhs) noexcept {
        std::size_t i = 0;
        for (; i < (element_number - 1); i++) {
            if (lhs.data[i] != rhs.data[i]) {
                return false;
            }
        }
        return (lhs.data[i] & last_mask) == (rhs.data[i] & last_mask);
    }

    [[nodiscard]]
    friend constexpr bool operator!=(const bitset &lhs, const bitset &rhs) noexcept {
        return not(lhs == rhs);
    }

    [[nodiscard]]
    constexpr bitset operator<<(std::size_t pos) const noexcept {
        bitset bs;
        const auto start = pos_to_y(pos);
        std::size_t i = 0;
        for (; i < start; i++) {
            bs.data[i] = 0;
        }
        pos = pos_to_x(pos);
        if (pos == 0) { // Aligned?
            for (; i < element_number; i++) {
                bs.data[i] = data[i - start];
            }
        } else {
            const auto shift = element_bit_width - pos;
            T lsb{};
            for (; i < element_number; i++) {
                T e = data[i - start];
                bs.data[i] = (e << pos) | lsb;
                lsb = e >> shift;
            }
        }
        return bs;
    }

    [[nodiscard]]
    constexpr bitset operator>>(std::size_t pos) const noexcept {
        bitset bs;
        const auto start = pos_to_y(pos);
        std::size_t i = start;

        pos = pos_to_x(pos);
        if (pos == 0) { // Aligned?
            for (; i < element_number; i++) {
                bs.data[i - start] = data[i];
            }
        } else {
            const auto shift = element_bit_width - pos;
            for (; i < (element_number - 1); i++) {
                bs.data[i - start] = (data[i] >> pos) | (data[i + 1] << shift);
            }
            bs.data[i - start] = (data[i] >> pos);
        }
        for (i = element_number - start; i < element_number; i++) {
            bs.data[i] = 0;
        }
        return bs;
    }

    constexpr bitset &operator<<=(std::size_t pos) noexcept {
        auto dst = element_number - 1;
        const auto start = dst - pos_to_y(pos);

        pos = pos_to_x(pos);
        if (pos == 0) { // Aligned?
            for (auto i = start; i > 0; i--) {
                data[dst--] = data[i];
            }
        } else {
            const auto shift = element_bit_width - pos;
            for (auto i = start; i > 0; i--) {
                data[dst] = data[i] << pos;
                data[dst--] |= (data[i - 1] >> shift);
            }
        }
        data[dst] = data[0] << pos;
        while (dst > 0) {
            data[--dst] = 0;
        }
        return *this;
    }

    constexpr bitset &operator>>=(std::size_t pos) noexcept {
        std::size_t dst = 0;
        const auto start = pos_to_y(pos);

        pos = pos_to_x(pos);
        if (pos == 0) { // Aligned?
            for (auto i = start; i < (element_number - 1); i++) {
                data[dst++] = data[i];
            }
        } else {
            auto const shift = element_bit_width - pos;
            for (auto i = start; i < (element_number - 1); i++) {
                data[dst] = data[i] >> pos;
                data[dst++] |= (data[i + 1] << shift);
            }
        }
        data[dst++] = data[element_number - 1] >> pos;
        while (dst < element_number) {
            data[dst++] = 0;
        }
        return *this;
    }

    constexpr bitset &operator&=(bitset &other) noexcept {
        // if (&other == this) {
        //     return *this;
        // }
        for (std::size_t i = 0; i < element_number; i++) {
            data[i] &= other.data[i];
        }
        return *this;
    }

    constexpr bitset &operator|=(const bitset &other) noexcept {
        // if (&other == this) {
        //     return *this;
        // }
        for (std::size_t i = 0; i < element_number; i++) {
            data[i] |= other.data[i];
        }
        return *this;
    }

    constexpr bitset &operator^=(const bitset &other) noexcept {
        // if (&other == this) {
        //     this->reset();
        //     return *this;
        // }
        for (std::size_t i = 0; i < element_number; i++) {
            data[i] ^= other.data[i];
        }
        return *this;
    }

    [[nodiscard]]
    constexpr bitset operator~() const noexcept {
        bitset bs;
        for (std::size_t i = 0; i < element_number; i++) {
            bs.data[i] = ~data[i];
        }
        return bs;
    }

    [[nodiscard]]
    friend constexpr bitset operator&(const bitset &lhs, const bitset &rhs) noexcept {
        bitset bs;
        for (std::size_t i = 0; i < element_number; i++) {
            bs.data[i] = lhs.data[i] & rhs.data[i];
        }
        return bs;
    }

    [[nodiscard]]
    friend constexpr bitset operator|(const bitset &lhs, const bitset &rhs) noexcept {
        bitset bs;
        for (std::size_t i = 0; i < element_number; i++) {
            bs.data[i] = lhs.data[i] | rhs.data[i];
        }
        return bs;
    }

    [[nodiscard]]
    friend constexpr bitset operator^(const bitset &lhs, const bitset &rhs) noexcept {
        bitset bs;
        for (std::size_t i = 0; i < element_number; i++) {
            bs.data[i] = lhs.data[i] ^ rhs.data[i];
        }
        return bs;
    }

    static constexpr bitset mask(std::size_t msb, std::size_t lsb) noexcept {
        const auto ly = pos_to_y(lsb);
        const auto lx = pos_to_x(lsb);

        const auto my = pos_to_y(msb);
        const auto mx = pos_to_x(msb);
        bitset bs;
        std::size_t i = 0;
        for (; i < ly; i++) {
            bs.data[i] = 0;
        }
        if (ly == my) {
            bs.data[i++] = kon::bit_mask<T>(mx, lx);
        } else {
            bs.data[i++] = kon::msb_mask<T>(lx);
            for (; i < my; i++) {
                bs.data[i] = ~static_cast<T>(0);
            }
            bs.data[i++] = kon::lsb_mask<T>(mx);
        }
        for (; i < element_number; i++) {
            bs.data[i] = 0;
        }
        return bs;
    }

    static constexpr bitset imask(std::size_t msb, std::size_t lsb) noexcept {
        auto ly = pos_to_y(lsb);
        auto lx = pos_to_x(lsb);

        auto my = pos_to_y(msb);
        auto mx = pos_to_x(msb);
        bitset bs;
        std::size_t i = 0;
        for (; i < ly; i++) {
            bs.data[i] = ~static_cast<T>(0);
        }
        if (ly == my) {
            bs.data[i++] = kon::bit_imask<T>(mx, lx);
        } else {
            bs.data[i++] = kon::msb_imask<T>(lx);
            for (; i < my; i++) {
                bs.data[i] = static_cast<T>(0);
            }
            bs.data[i++] = kon::lsb_imask<T>(mx);
        }
        for (; i < element_number; i++) {
            bs.data[i] = ~static_cast<T>(0);
        }
        return bs;
    }
};

} // namespace kon

#endif // bitset.hpp