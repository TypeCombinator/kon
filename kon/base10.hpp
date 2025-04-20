#ifndef BASE10_1775C9DF_D47A_4AE2_AF44_36E468E90A24
#define BASE10_1775C9DF_D47A_4AE2_AF44_36E468E90A24
#include <cstdint>

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

static constexpr bool is_base10_char(char c) noexcept {
    return static_cast<std::uint8_t>(c - '0') < 10;
}

static constexpr std::uint8_t base10_char_decode(char c) noexcept {
    return static_cast<std::uint8_t>(c - '0');
}
} // namespace kon
#endif /* base10.hpp */