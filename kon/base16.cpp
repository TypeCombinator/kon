#include <kon/base16.hpp>
#include <type_traits>
#include <limits>

namespace kon {

const uint8_t base16_encode_table[16] =
    {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

const uint8_t base16_decode_table[256] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

template <typename T>
uint8_t rstring10_to_general_uint(const char *str, std::size_t str_size, T &result) {
    // if (str_size == 0) {
    //     return 0;
    // }
    const char *str_org = str;
    // Remove zeros.
    for (; str_size > 0; str_size--) {
        if (*str != '0') {
            break;
        }
        str++;
    }
    T number = 0;
    constexpr auto max_size = std::numeric_limits<T>::digits10;
    std::size_t s = (str_size <= max_size) ? str_size : max_size;
    uint8_t c;
    for (; s > 0; s--) {
        c = base16_decode_table[static_cast<uint8_t>(*str)];
        number = number * 10 + c;
        if (c >= 10) {
            result = number;
            return str - str_org;
        }
        str++;
    }
    if (str_size <= max_size) {
        result = number;
        return str - str_org;
    } // else if (str_size > max_size)
    c = base16_decode_table[static_cast<uint8_t>(*str)];
    if (c >= 10) {
        result = number;
        return str - str_org;
    }
    constexpr T max_d10 = std::numeric_limits<T>::max() / 10;
    constexpr uint8_t max_r10 = std::numeric_limits<T>::max() % 10;
    if ((number > max_d10) || ((number == max_d10) && (c > max_r10))) { // Overflow?
        return 0;
    }
    number = number * 10 + c;

    str++;
    str_size -= (max_size + 1);
    if ((str_size != 0) & is_base10(*str)) { // Too many?
        return 0;
    }
    result = number;
    return str - str_org;
}

uint8_t rstring10_to_uint(const char *str, std::size_t str_size, uint8_t &result) {
    return rstring10_to_general_uint(str, str_size, result);
}

uint8_t rstring10_to_uint(const char *str, std::size_t str_size, uint16_t &result) {
    return rstring10_to_general_uint(str, str_size, result);
}

uint8_t rstring10_to_uint(const char *str, std::size_t str_size, uint32_t &result) {
    return rstring10_to_general_uint(str, str_size, result);
}

uint8_t rstring10_to_uint(const char *str, std::size_t str_size, uint64_t &result) {
    return rstring10_to_general_uint(str, str_size, result);
}

template <typename T>
uint8_t rstring16_to_general_uint(const char *str, std::size_t str_size, T &result) {
    const char *str_org = str;
    // Remove zeros.
    for (; str_size > 0; str_size--) {
        if (*str != '0') {
            break;
        }
        str++;
    }
    T number = 0;
    constexpr auto max_size = sizeof(T) * 2;
    std::size_t s = (str_size <= max_size) ? str_size : max_size;
    uint8_t c;
    for (; s > 0; s--) {
        c = base16_decode_table[static_cast<uint8_t>(*str)];
        number = (number << 4) + c;
        if (c >= 16) {
            result = number;
            return str - str_org;
        }
        str++;
    }
    if (str_size <= max_size) {
        result = number;
        return str - str_org;
    } // else if (str_size > max_size)
    if (is_base16(*str)) { // Too many?
        return 0;
    }
    result = number;
    return str - str_org;
}

uint8_t rstring16_to_uint(const char *str, std::size_t str_size, uint8_t &result) {
    return rstring16_to_general_uint(str, str_size, result);
}

uint8_t rstring16_to_uint(const char *str, std::size_t str_size, uint16_t &result) {
    return rstring16_to_general_uint(str, str_size, result);
}

uint8_t rstring16_to_uint(const char *str, std::size_t str_size, uint32_t &result) {
    return rstring16_to_general_uint(str, str_size, result);
}

uint8_t rstring16_to_uint(const char *str, std::size_t str_size, uint64_t &result) {
    return rstring16_to_general_uint(str, str_size, result);
}

template <typename T>
uint8_t string10_to_general_uint(const char *str, std::size_t str_size, T &result) {
    if (str_size < 1) {
        return 0;
    }
    uint8_t prefix = (*str == '+') ? 1 : 0;
    uint8_t pos = rstring10_to_general_uint(str + prefix, str_size - prefix, result);
    return (pos != 0) ? pos + prefix : 0;
}

uint8_t string10_to_uint(const char *str, std::size_t str_size, uint8_t &result) {
    return string10_to_general_uint(str, str_size, result);
}

uint8_t string10_to_uint(const char *str, std::size_t str_size, uint16_t &result) {
    return string10_to_general_uint(str, str_size, result);
}

uint8_t string10_to_uint(const char *str, std::size_t str_size, uint32_t &result) {
    return string10_to_general_uint(str, str_size, result);
}

uint8_t string10_to_uint(const char *str, std::size_t str_size, uint64_t &result) {
    return string10_to_general_uint(str, str_size, result);
}

template <typename T>
uint8_t string16_to_general_uint(const char *str, std::size_t str_size, T &result) {
    if (str_size < 3) {
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
    char c = str[1];
    if (*str != '0' || ((c != 'x') && (c != 'X'))) {
        return 0;
    }
    uint8_t pos = rstring16_to_general_uint(str_org + prefix, str_size - prefix, result);
    return (pos != 0) ? (pos + prefix) : 0;
}

uint8_t string16_to_uint(const char *str, std::size_t str_size, uint8_t &result) {
    return string16_to_general_uint(str, str_size, result);
}

uint8_t string16_to_uint(const char *str, std::size_t str_size, uint16_t &result) {
    return string16_to_general_uint(str, str_size, result);
}

uint8_t string16_to_uint(const char *str, std::size_t str_size, uint32_t &result) {
    return string16_to_general_uint(str, str_size, result);
}

uint8_t string16_to_uint(const char *str, std::size_t str_size, uint64_t &result) {
    return string16_to_general_uint(str, str_size, result);
}

template <typename T>
uint8_t string10_to_general_int(const char *str, std::size_t str_size, T &result) {
    if (str_size < 1) {
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
    uint8_t pos = rstring10_to_general_uint(str + prefix, str_size - prefix, number);
    if (pos == 0) {
        return 0;
    }
    if (negative) {
        if (number > static_cast<UT>(std::numeric_limits<T>::min())) {
            return 0;
        }
        result = -number;
        return prefix + pos;
    }
    if (number > static_cast<UT>(std::numeric_limits<T>::max())) {
        return 0;
    }
    result = number;
    return prefix + pos;
}

uint8_t string10_to_int(const char *str, std::size_t str_size, int8_t &result) {
    return string10_to_general_int(str, str_size, result);
}

uint8_t string10_to_int(const char *str, std::size_t str_size, int16_t &result) {
    return string10_to_general_int(str, str_size, result);
}

uint8_t string10_to_int(const char *str, std::size_t str_size, int32_t &result) {
    return string10_to_general_int(str, str_size, result);
}

uint8_t string10_to_int(const char *str, std::size_t str_size, int64_t &result) {
    return string10_to_general_int(str, str_size, result);
}

template <typename T>
uint8_t string16_to_general_int(const char *str, std::size_t str_size, T &result) {
    if (str_size < 3) {
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
    c = str[1];
    if (*str != '0' || ((c != 'x') && (c != 'X'))) {
        return 0;
    }
    using UT = std::make_unsigned_t<T>;
    UT number;
    uint8_t pos = rstring16_to_general_uint(str_org + prefix, str_size - prefix, number);
    if (pos == 0) {
        return 0;
    }
    if (negative) {
        if (number > static_cast<UT>(std::numeric_limits<T>::min())) {
            return 0;
        }
        result = -number;
        return prefix + pos;
    }
    if (number > static_cast<UT>(std::numeric_limits<T>::max())) {
        return 0;
    }
    result = number;
    return prefix + pos;
}

uint8_t string16_to_int(const char *str, std::size_t str_size, int8_t &result) {
    return string16_to_general_int(str, str_size, result);
}

uint8_t string16_to_int(const char *str, std::size_t str_size, int16_t &result) {
    return string16_to_general_int(str, str_size, result);
}

uint8_t string16_to_int(const char *str, std::size_t str_size, int32_t &result) {
    return string16_to_general_int(str, str_size, result);
}

uint8_t string16_to_int(const char *str, std::size_t str_size, int64_t &result) {
    return string16_to_general_int(str, str_size, result);
}

uint8_t string_to_float(const char *str, std::size_t str_size, float result) {
    return 0;
}

} // namespace kon