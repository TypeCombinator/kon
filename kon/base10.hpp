// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef BASE10_1775C9DF_D47A_4AE2_AF44_36E468E90A24
#define BASE10_1775C9DF_D47A_4AE2_AF44_36E468E90A24
#include <cstdint>
#include <kon/bit.hpp>

namespace kon {

template <typename T, T limit>
constexpr T devide10_limit_lut[10] = {
    (limit - 0) / 10,
    (limit - 1) / 10,
    (limit - 2) / 10,
    (limit - 3) / 10,
    (limit - 4) / 10,
    (limit - 5) / 10,
    (limit - 6) / 10,
    (limit - 7) / 10,
    (limit - 8) / 10,
    (limit - 9) / 10,
};

namespace detail {
static constexpr uint8_t base10_count_lut[65] = {
    19, 19, 19, 19, 18, 18, 18, 17, //
    17, 17, 16, 16, 16, 16, 15, 15, //
    15, 14, 14, 14, 13, 13, 13, 13, //
    12, 12, 12, 11, 11, 11, 10, 10, //
    10, 10, 9,  9,  9,  8,  8,  8,  //
    7,  7,  7,  7,  6,  6,  6,  5,  //
    5,  5,  4,  4,  4,  4,  3,  3,  //
    3,  2,  2,  2,  1,  1,  1,  1,  1,
};
static constexpr uint64_t base10_count_threshold_lut[20] = {
    0ull,
    9ull,
    99ull,
    999ull,
    9999ull,
    99999ull,
    999999ull,
    9999999ull,
    99999999ull,
    999999999ull,
    9999999999ull,
    99999999999ull,
    999999999999ull,
    9999999999999ull,
    99999999999999ull,
    999999999999999ull,
    9999999999999999ull,
    99999999999999999ull,
    999999999999999999ull,
    9999999999999999999ull,
};
} // namespace detail

static constexpr bool is_base10_char(char c) noexcept {
    return static_cast<std::uint8_t>(c - '0') < 10;
}

static constexpr std::uint8_t base10_char_decode(char c) noexcept {
    return static_cast<std::uint8_t>(c - '0');
}

static constexpr std::uint8_t base10_count(const std::uint64_t inputValue) noexcept {
    const std::uint8_t count = detail::base10_count_lut[kon::countl_zero(inputValue)];
    return count + static_cast<uint8_t>(inputValue > detail::base10_count_threshold_lut[count]);
}

} // namespace kon
#endif // base10.hpp