// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef BIT_4E5B20B2_B9E4_4DE7_B12A_4D0D6BCA298E
#define BIT_4E5B20B2_B9E4_4DE7_B12A_4D0D6BCA298E

namespace kon {

template <typename T>
constexpr T byteswap(T x) noexcept {
    if constexpr (sizeof(x) == 2) {
        return __builtin_bswap16(x);
    } else if constexpr (sizeof(x) == 4) {
        return __builtin_bswap32(x);
    } else if constexpr (sizeof(x) == 8) {
        return __builtin_bswap64(x);
    } else {
        static_assert(sizeof(x) <= 8);
        return 0;
    }
}

template <typename T, bool enable_ub = false>
constexpr int countl_zero(T x) noexcept {
    if constexpr (!enable_ub) {
        if (x == 0) { // __builtin_clz(0) is UB!
            return sizeof(x) * 8;
        }
    }
    if constexpr (sizeof(x) <= sizeof(unsigned)) {
        return __builtin_clz(x);
    } else if constexpr (sizeof(x) <= sizeof(unsigned long)) {
        return __builtin_clzl(x);
    } else if constexpr (sizeof(x) <= sizeof(unsigned long long)) {
        return __builtin_clzll(x);
    } else {
        static_assert(sizeof(x) <= sizeof(unsigned long long));
        return -1;
    }
}

template <typename T, bool enable_ub = false>
constexpr int countr_zero(T x) noexcept {
    if constexpr (!enable_ub) {
        if (x == 0) { // __builtin_ctz(0) is UB!
            return sizeof(x) * 8;
        }
    }
    if constexpr (sizeof(x) <= sizeof(unsigned)) {
        return __builtin_ctz(x);
    } else if constexpr (sizeof(x) <= sizeof(unsigned long)) {
        return __builtin_ctzl(x);
    } else if constexpr (sizeof(x) <= sizeof(unsigned long long)) {
        return __builtin_ctzll(x);
    } else {
        static_assert(sizeof(x) <= sizeof(unsigned long long));
        return -1;
    }
}

template <typename T>
constexpr int popcount(T x) noexcept {
    if constexpr (sizeof(x) <= sizeof(unsigned)) {
        return __builtin_popcount(x);
    } else if constexpr (sizeof(x) <= sizeof(unsigned long)) {
        return __builtin_popcountl(x);
    } else if constexpr (sizeof(x) <= sizeof(unsigned long long)) {
        return __builtin_popcountll(x);
    } else {
        static_assert(sizeof(x) <= sizeof(unsigned long long));
        return -1;
    }
}

// T: uint8_t, pos: 3 --> 1111_0000
template <typename T>
static constexpr T lsb_imask(unsigned char pos) noexcept {
    return (~static_cast<T>(0) << (pos + 1));
}

// T: uint8_t, pos: 3 --> 0000_1111
template <typename T>
static constexpr T lsb_mask(unsigned char pos) noexcept {
    return (static_cast<T>(1) << (pos + 1)) - 1;
}

// T: uint8_t, pos: 3 --> 1111_1000
template <typename T>
static constexpr T msb_mask(unsigned char pos) noexcept {
    return (~static_cast<T>(0)) << pos;
}

// T: uint8_t, pos: 3 --> 0000_0111
template <typename T>
static constexpr T msb_imask(unsigned char pos) noexcept {
    return (static_cast<T>(1) << pos) - 1;
}

// T: uint8_t, msb: 6, lsb: 3 --> 0111_1000 (<= 1000_0000 ^ 1111_1000)
template <typename T>
static constexpr T bit_mask(unsigned char msb, unsigned char lsb) noexcept {
    return msb_mask<T>(msb + 1) ^ msb_mask<T>(lsb);
}

// T: uint8_t, msb: 6, lsb: 3 --> 1000_0111 (<= 1000_0000 | 0000_0111)
template <typename T>
static constexpr T bit_imask(unsigned char msb, unsigned char lsb) noexcept {
    return msb_mask<T>(msb + 1) | msb_imask<T>(lsb);
}

} // namespace kon
#endif // bit.hpp