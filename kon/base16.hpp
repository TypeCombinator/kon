// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef BASE16_46017852_440D_4C90_9265_E30FCAC05E2E
#define BASE16_46017852_440D_4C90_9265_E30FCAC05E2E
#include <cstdint>

namespace kon {
extern const uint8_t base16_encode_table[16];
extern const uint8_t base16_decode_table[256];
extern const char base16_encode_lut2[512];

static inline bool is_base16_char(char c) noexcept {
    return base16_decode_table[static_cast<uint8_t>(c)] < 16;
}

static inline std::uint8_t base16_char_decode(char c) noexcept {
    return base16_decode_table[static_cast<uint8_t>(c)];
}

std::size_t base16_decode(
    const char *str,
    std::size_t str_size,
    std::uint8_t *data,
    std::size_t data_size) noexcept;

}; // namespace kon
#endif /* base16.hpp */