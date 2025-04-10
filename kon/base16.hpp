#ifndef BASE16_46017852_440D_4C90_9265_E30FCAC05E2E
#define BASE16_46017852_440D_4C90_9265_E30FCAC05E2E
#include <cstdint>

namespace kon {
extern const uint8_t base16_encode_table[16];
extern const uint8_t base16_decode_table[256];

static inline bool is_base10(char c) {
    return base16_decode_table[static_cast<uint8_t>(c)] < 10;
}

static inline bool is_base16(char c) {
    return base16_decode_table[static_cast<uint8_t>(c)] < 16;
}

uint8_t rstring10_to_uint(const char *str, std::size_t str_size, uint8_t &result);
uint8_t rstring10_to_uint(const char *str, std::size_t str_size, uint16_t &result);
uint8_t rstring10_to_uint(const char *str, std::size_t str_size, uint32_t &result);
uint8_t rstring10_to_uint(const char *str, std::size_t str_size, uint64_t &result);

uint8_t rstring16_to_uint(const char *str, std::size_t str_size, uint8_t &result);
uint8_t rstring16_to_uint(const char *str, std::size_t str_size, uint16_t &result);
uint8_t rstring16_to_uint(const char *str, std::size_t str_size, uint32_t &result);
uint8_t rstring16_to_uint(const char *str, std::size_t str_size, uint64_t &result);


uint8_t string10_to_uint(const char *str, std::size_t str_size, uint8_t &result);
uint8_t string10_to_uint(const char *str, std::size_t str_size, uint16_t &result);
uint8_t string10_to_uint(const char *str, std::size_t str_size, uint32_t &result);
uint8_t string10_to_uint(const char *str, std::size_t str_size, uint64_t &result);

uint8_t string16_to_uint(const char *str, std::size_t str_size, uint8_t &result);
uint8_t string16_to_uint(const char *str, std::size_t str_size, uint16_t &result);
uint8_t string16_to_uint(const char *str, std::size_t str_size, uint32_t &result);
uint8_t string16_to_uint(const char *str, std::size_t str_size, uint64_t &result);


uint8_t string10_to_int(const char *str, std::size_t str_size, int8_t &result);
uint8_t string10_to_int(const char *str, std::size_t str_size, int16_t &result);
uint8_t string10_to_int(const char *str, std::size_t str_size, int32_t &result);
uint8_t string10_to_int(const char *str, std::size_t str_size, int64_t &result);

uint8_t string16_to_int(const char *str, std::size_t str_size, int8_t &result);
uint8_t string16_to_int(const char *str, std::size_t str_size, int16_t &result);
uint8_t string16_to_int(const char *str, std::size_t str_size, int32_t &result);
uint8_t string16_to_int(const char *str, std::size_t str_size, int64_t &result);

uint8_t string_to_float(const char *str, std::size_t str_size, float &result);
uint8_t string_to_float(const char *str, std::size_t str_size, double &result);
uint8_t string_to_float(const char *str, std::size_t str_size, long double &result);

}; // namespace kon
#endif /* base16.hpp */