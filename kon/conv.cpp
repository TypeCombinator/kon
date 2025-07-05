// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#include <kon/conv.hpp>
#include <kon/base10.hpp>
#include <kon/base16.hpp>
#include <cstring>
#include <limits>
#include <charconv>
#include <bit>

namespace kon {
template <typename T>
static inline std::size_t
    rstring10_to_general_uint(const char *str, std::size_t str_size, T &result) noexcept {
    const char *str_org = str;
    // Remove zeros.
    for (; str_size > 0; str_size--) {
        if (*str != '0') [[likely]] {
            break;
        }
        str++;
    }
    T number = 0;
    constexpr auto max_size = std::numeric_limits<T>::digits10;
    uint8_t c;
    for (std::size_t s = (str_size <= max_size) ? str_size : max_size; s > 0; s--) {
        c = base10_char_decode(*str);
        if (c >= 10) [[unlikely]] {
            result = number;
            return str - str_org;
        }
        number = number * 10 + c;
        str++;
    }
    if (str_size <= max_size) [[likely]] {
        result = number;
        return str - str_org;
    } // else if (str_size > max_size)
    c = base10_char_decode(*str);
    if (c >= 10) [[unlikely]] {
        result = number;
        return str - str_org;
    }
    if (number > devide10_limit_lut<T, std::numeric_limits<T>::max()>[c]) [[unlikely]] {
        return 0;
    }

    str++;
    str_size -= (max_size + 1);
    if ((str_size != 0) && is_base10_char(*str)) [[unlikely]] { // Too many?
        return 0;
    }
    result = number * 10 + c;
    return str - str_org;
}

std::size_t rstring10_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept {
    return rstring10_to_general_uint(str, str_size, result);
}

std::size_t rstring10_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept {
    return rstring10_to_general_uint(str, str_size, result);
}

std::size_t rstring10_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept {
    return rstring10_to_general_uint(str, str_size, result);
}

std::size_t rstring10_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept {
    return rstring10_to_general_uint(str, str_size, result);
}

template <typename T>
static inline std::size_t
    rstring16_to_general_uint(const char *str, std::size_t str_size, T &result) noexcept {
    const char *str_org = str;
    // Remove zeros.
    for (; str_size > 0; str_size--) {
        if (*str != '0') [[likely]] {
            break;
        }
        str++;
    }
    T number = 0;
    constexpr auto max_size = sizeof(T) * 2;
    uint8_t c;
    for (std::size_t s = (str_size <= max_size) ? str_size : max_size; s > 0; s--) {
        c = base16_char_decode(*str);
        if (c >= 16) [[unlikely]] {
            result = number;
            return str - str_org;
        }
        number = (number << 4) + c;
        str++;
    }
    if (str_size <= max_size) [[likely]] {
        result = number;
        return str - str_org;
    } // else if (str_size > max_size)
    if (is_base16_char(*str)) [[unlikely]] { // Too many?
        return 0;
    }
    result = number;
    return str - str_org;
}

std::size_t rstring16_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept {
    return rstring16_to_general_uint(str, str_size, result);
}

std::size_t rstring16_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept {
    return rstring16_to_general_uint(str, str_size, result);
}

std::size_t rstring16_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept {
    return rstring16_to_general_uint(str, str_size, result);
}

std::size_t rstring16_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept {
    return rstring16_to_general_uint(str, str_size, result);
}

template <typename T>
static inline std::size_t
    string10_to_general_uint(const char *str, std::size_t str_size, T &result) noexcept {
    if (str_size < 1) [[unlikely]] {
        return 0;
    }
    uint8_t prefix = (*str == '+') ? 1 : 0;
    std::size_t pos = rstring10_to_general_uint(str + prefix, str_size - prefix, result);
    return (pos != 0) ? pos + prefix : 0;
}

std::size_t string10_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept {
    return string10_to_general_uint(str, str_size, result);
}

std::size_t string10_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept {
    return string10_to_general_uint(str, str_size, result);
}

std::size_t string10_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept {
    return string10_to_general_uint(str, str_size, result);
}

std::size_t string10_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept {
    return string10_to_general_uint(str, str_size, result);
}

static inline bool is_string_start_witch_0x(const char *str) noexcept {
    // static_cast<const uint8_t>(data[0]) | (static_cast<const uint8_t>(data[1]) << 8);
    uint16_t prefix;
    std::memcpy(&prefix, str, sizeof(prefix));
    if constexpr (std::endian::native == std::endian::little) {
        return (prefix | 0x2000) == 0x7830;
    } else {
        return (prefix | 0x20) == 0x3078;
    }
}

template <typename T>
static std::size_t
    string16_to_general_uint(const char *str, std::size_t str_size, T &result) noexcept {
    if (str_size < 3) [[unlikely]] {
        return 0;
    }
    const char *str_org = str;
    uint8_t prefix;
    if (*str == '+') {
        str++;
        prefix = 3;
    } else {
        prefix = 2;
    }
    if (!is_string_start_witch_0x(str)) [[unlikely]] {
        return 0;
    }
    std::size_t pos = rstring16_to_general_uint(str_org + prefix, str_size - prefix, result);
    return (pos != 0) ? (pos + prefix) : 0;
}

std::size_t string16_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept {
    return string16_to_general_uint(str, str_size, result);
}

std::size_t string16_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept {
    return string16_to_general_uint(str, str_size, result);
}

std::size_t string16_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept {
    return string16_to_general_uint(str, str_size, result);
}

std::size_t string16_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept {
    return string16_to_general_uint(str, str_size, result);
}

template <typename T>
static inline std::size_t
    string10_to_general_int(const char *str, std::size_t str_size, T &result) noexcept {
    if (str_size < 1) [[unlikely]] {
        return 0;
    }
    uint8_t prefix = 0;
    char c = *str;
    bool negative = false;
    if (c == '-') {
        prefix = 1;
        negative = true;
    } else if (c == '+') {
        prefix = 1;
    }
    using UT = std::make_unsigned_t<T>;
    UT number;
    std::size_t pos = rstring10_to_general_uint(str + prefix, str_size - prefix, number);
    if (pos == 0) [[unlikely]] {
        return 0;
    }
    if (negative) {
        if (number > static_cast<UT>(std::numeric_limits<T>::min())) [[unlikely]] {
            return 0;
        }
        result = -number;
        return prefix + pos;
    }
    if (number > static_cast<UT>(std::numeric_limits<T>::max())) [[unlikely]] {
        return 0;
    }
    result = number;
    return prefix + pos;
}

std::size_t string10_to_int(const char *str, std::size_t str_size, int8_t &result) noexcept {
    return string10_to_general_int(str, str_size, result);
}

std::size_t string10_to_int(const char *str, std::size_t str_size, int16_t &result) noexcept {
    return string10_to_general_int(str, str_size, result);
}

std::size_t string10_to_int(const char *str, std::size_t str_size, int32_t &result) noexcept {
    return string10_to_general_int(str, str_size, result);
}

std::size_t string10_to_int(const char *str, std::size_t str_size, int64_t &result) noexcept {
    return string10_to_general_int(str, str_size, result);
}

template <typename T>
static inline std::size_t
    string16_to_general_int(const char *str, std::size_t str_size, T &result) noexcept {
    if (str_size < 3) [[unlikely]] {
        return 0;
    }
    const char *str_org = str;
    uint8_t prefix;
    bool negative = false;
    char c = *str;
    if (c == '-') {
        str++;
        prefix = 3;
        negative = true;
    } else if (c == '+') {
        str++;
        prefix = 3;
    } else {
        prefix = 2;
    }
    if (!is_string_start_witch_0x(str)) [[unlikely]] {
        return 0;
    }
    using UT = std::make_unsigned_t<T>;
    UT number;
    std::size_t pos = rstring16_to_general_uint(str_org + prefix, str_size - prefix, number);
    if (pos == 0) [[unlikely]] {
        return 0;
    }
    if (negative) {
        if (number > static_cast<UT>(std::numeric_limits<T>::min())) [[unlikely]] {
            return 0;
        }
        result = -number;
        return prefix + pos;
    }
    if (number > static_cast<UT>(std::numeric_limits<T>::max())) [[unlikely]] {
        return 0;
    }
    result = number;
    return prefix + pos;
}

std::size_t string16_to_int(const char *str, std::size_t str_size, int8_t &result) noexcept {
    return string16_to_general_int(str, str_size, result);
}

std::size_t string16_to_int(const char *str, std::size_t str_size, int16_t &result) noexcept {
    return string16_to_general_int(str, str_size, result);
}

std::size_t string16_to_int(const char *str, std::size_t str_size, int32_t &result) noexcept {
    return string16_to_general_int(str, str_size, result);
}

std::size_t string16_to_int(const char *str, std::size_t str_size, int64_t &result) noexcept {
    return string16_to_general_int(str, str_size, result);
}

template <typename T>
std::size_t string_to_general_float(const char *str, std::size_t str_size, T &result) noexcept {
    auto &&ret = std::from_chars(str, str + str_size, result);
    return (ret.ec == std::errc()) ? (ret.ptr - str) : 0;
}

std::size_t string_to_float(const char *str, std::size_t str_size, float &result) noexcept {
    return string_to_general_float(str, str_size, result);
}

std::size_t string_to_float(const char *str, std::size_t str_size, double &result) noexcept {
    return string_to_general_float(str, str_size, result);
}

std::size_t string_to_float(const char *str, std::size_t str_size, long double &result) noexcept {
    return string_to_general_float(str, str_size, result);
}

template <typename T>
static inline std::size_t
    string_to_general_uint(const char *str, std::size_t str_size, T &result) noexcept {
    const char *str_org = str;
    if (str_size > 0) [[likely]] {
        uint8_t c = *str;
        if (c == '+') {
            str++;
            str_size--;
        }
    }
    if (str_size >= 2) {
        if (is_string_start_witch_0x(str)) {
            str += 2;
            std::size_t pos = rstring16_to_general_uint(str, str_size - 2, result);
            if (pos == 0) [[unlikely]] {
                return 0;
            }
            return (str - str_org) + pos;
        }
    }
    std::size_t pos = rstring10_to_general_uint(str, str_size, result);
    if (pos == 0) [[unlikely]] {
        return 0;
    }
    return (str - str_org) + pos;
}

std::size_t string_to_uint(const char *str, std::size_t str_size, uint8_t &result) noexcept {
    return string_to_general_uint(str, str_size, result);
}

std::size_t string_to_uint(const char *str, std::size_t str_size, uint16_t &result) noexcept {
    return string_to_general_uint(str, str_size, result);
}

std::size_t string_to_uint(const char *str, std::size_t str_size, uint32_t &result) noexcept {
    return string_to_general_uint(str, str_size, result);
}

std::size_t string_to_uint(const char *str, std::size_t str_size, uint64_t &result) noexcept {
    return string_to_general_uint(str, str_size, result);
}

template <typename T>
static inline std::size_t
    string_to_general_int(const char *str, std::size_t str_size, T &result) noexcept {
    const char *str_org = str;
    bool negative = false;
    if (str_size > 0) [[likely]] {
        uint8_t c = *str;
        if (c == '+') {
            str++;
            str_size--;
        } else if (c == '-') {
            str++;
            str_size--;
            negative = true;
        }
    }
    using UT = std::make_unsigned_t<T>;
    UT number;
    if (str_size >= 2) {
        if (is_string_start_witch_0x(str)) {
            str += 2;
            std::size_t pos = rstring16_to_general_uint(str, str_size - 2, number);
            if (pos == 0) [[unlikely]] {
                return 0;
            }
            if (negative) {
                if (number > static_cast<UT>(std::numeric_limits<T>::min())) [[unlikely]] {
                    return 0;
                }
                result = -number;
                return (str - str_org) + pos;
            }
            if (number > static_cast<UT>(std::numeric_limits<T>::max())) [[unlikely]] {
                return 0;
            }
            result = number;
            return (str - str_org) + pos;
        }
    }
    std::size_t pos = rstring10_to_general_uint(str, str_size, number);
    if (pos == 0) {
        return 0;
    }
    if (negative) {
        if (number > static_cast<UT>(std::numeric_limits<T>::min())) [[unlikely]] {
            return 0;
        }
        result = -number;
        return (str - str_org) + pos;
    }
    if (number > static_cast<UT>(std::numeric_limits<T>::max())) [[unlikely]] {
        return 0;
    }
    result = number;
    return (str - str_org) + pos;
}

std::size_t string_to_int(const char *str, std::size_t str_size, int8_t &result) noexcept {
    return string_to_general_int(str, str_size, result);
}

std::size_t string_to_int(const char *str, std::size_t str_size, int16_t &result) noexcept {
    return string_to_general_int(str, str_size, result);
}

std::size_t string_to_int(const char *str, std::size_t str_size, int32_t &result) noexcept {
    return string_to_general_int(str, str_size, result);
}

std::size_t string_to_int(const char *str, std::size_t str_size, int64_t &result) noexcept {
    return string_to_general_int(str, str_size, result);
}

} // namespace kon