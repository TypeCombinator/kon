// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef CONV_14F2E883_1F2D_4930_BB8D_69E248D3B083
#define CONV_14F2E883_1F2D_4930_BB8D_69E248D3B083
#include <cstdint>

namespace kon {

extern const uint8_t base16_decode_table[256];

static inline bool is_base10(char c) noexcept {
    return base16_decode_table[static_cast<uint8_t>(c)] < 10;
}

static inline bool is_base16(char c) noexcept {
    return base16_decode_table[static_cast<uint8_t>(c)] < 16;
}

uint8_t rstring10_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept;
uint8_t rstring10_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept;
uint8_t rstring10_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept;
uint8_t rstring10_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept;

uint8_t rstring16_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept;
uint8_t rstring16_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept;
uint8_t rstring16_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept;
uint8_t rstring16_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept;


uint8_t string10_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept;
uint8_t string10_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept;
uint8_t string10_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept;
uint8_t string10_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept;

uint8_t string16_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept;
uint8_t string16_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept;
uint8_t string16_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept;
uint8_t string16_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept;


uint8_t string10_to_int(const char *str, std::size_t str_size, int8_t &result) noexcept;
uint8_t string10_to_int(const char *str, std::size_t str_size, int16_t &result) noexcept;
uint8_t string10_to_int(const char *str, std::size_t str_size, int32_t &result) noexcept;
uint8_t string10_to_int(const char *str, std::size_t str_size, int64_t &result) noexcept;

uint8_t string16_to_int(const char *str, std::size_t str_size, int8_t &result) noexcept;
uint8_t string16_to_int(const char *str, std::size_t str_size, int16_t &result) noexcept;
uint8_t string16_to_int(const char *str, std::size_t str_size, int32_t &result) noexcept;
uint8_t string16_to_int(const char *str, std::size_t str_size, int64_t &result) noexcept;


uint8_t string_to_float(const char *str, std::size_t str_size, float &result) noexcept;
uint8_t string_to_float(const char *str, std::size_t str_size, double &result) noexcept;
uint8_t string_to_float(const char *str, std::size_t str_size, long double &result) noexcept;


uint8_t string_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept;
uint8_t string_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept;
uint8_t string_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept;
uint8_t string_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept;


uint8_t string_to_int(const char *str, std::size_t str_size, int8_t &result) noexcept;
uint8_t string_to_int(const char *str, std::size_t str_size, int16_t &result) noexcept;
uint8_t string_to_int(const char *str, std::size_t str_size, int32_t &result) noexcept;
uint8_t string_to_int(const char *str, std::size_t str_size, int64_t &result) noexcept;

}; // namespace kon
#endif /* conv.hpp */