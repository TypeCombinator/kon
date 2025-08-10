// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef CONV_14F2E883_1F2D_4930_BB8D_69E248D3B083
#define CONV_14F2E883_1F2D_4930_BB8D_69E248D3B083
#include <cstdint>

namespace kon {

std::size_t rstring10_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept;
std::size_t rstring10_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept;
std::size_t rstring10_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept;
std::size_t rstring10_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept;

std::size_t rstring16_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept;
std::size_t rstring16_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept;
std::size_t rstring16_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept;
std::size_t rstring16_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept;


std::size_t string10_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept;
std::size_t string10_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept;
std::size_t string10_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept;
std::size_t string10_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept;

std::size_t string16_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept;
std::size_t string16_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept;
std::size_t string16_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept;
std::size_t string16_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept;


std::size_t string10_to_int(const char *str, std::size_t str_size, int8_t &result) noexcept;
std::size_t string10_to_int(const char *str, std::size_t str_size, int16_t &result) noexcept;
std::size_t string10_to_int(const char *str, std::size_t str_size, int32_t &result) noexcept;
std::size_t string10_to_int(const char *str, std::size_t str_size, int64_t &result) noexcept;

std::size_t string16_to_int(const char *str, std::size_t str_size, int8_t &result) noexcept;
std::size_t string16_to_int(const char *str, std::size_t str_size, int16_t &result) noexcept;
std::size_t string16_to_int(const char *str, std::size_t str_size, int32_t &result) noexcept;
std::size_t string16_to_int(const char *str, std::size_t str_size, int64_t &result) noexcept;


std::size_t string_to_float(const char *str, std::size_t str_size, float &result) noexcept;
std::size_t string_to_float(const char *str, std::size_t str_size, double &result) noexcept;
std::size_t string_to_float(const char *str, std::size_t str_size, long double &result) noexcept;


std::size_t string_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept;
std::size_t string_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept;
std::size_t string_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept;
std::size_t string_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept;


std::size_t string_to_int(const char *str, std::size_t str_size, int8_t &result) noexcept;
std::size_t string_to_int(const char *str, std::size_t str_size, int16_t &result) noexcept;
std::size_t string_to_int(const char *str, std::size_t str_size, int32_t &result) noexcept;
std::size_t string_to_int(const char *str, std::size_t str_size, int64_t &result) noexcept;

}; // namespace kon
#endif /* conv.hpp */