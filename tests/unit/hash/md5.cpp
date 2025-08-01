#include <kon/hash/md5.hpp>
#include <string_view>
#include <catch2/catch_test_macros.hpp>
#include <kon/types.hpp>

static inline bool md5_test_helper(std::string_view str, const unsigned char check[16]) noexcept {
    unsigned char output[16];
    kon::md5(reinterpret_cast<const unsigned char *>(str.data()), str.length(), output);
    return memcmp(output, check, sizeof(output)) == 0;
}

using namespace std::string_view_literals;

static const std::string_view md5_test_buf[] = {
    ""sv,
    "a"sv,
    "abc"sv,
    "message digest"sv,
    "abcdefghijklmnopqrstuvwxyz"sv,
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"sv,
    "12345678901234567890123456789012345678901234567890123456789012345678901234567890"sv,
};

static const unsigned char md5_test_sum[][16] = {
    {0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04, 0xE9, 0x80, 0x09, 0x98, 0xEC, 0xF8, 0x42, 0x7E},
    {0x0C, 0xC1, 0x75, 0xB9, 0xC0, 0xF1, 0xB6, 0xA8, 0x31, 0xC3, 0x99, 0xE2, 0x69, 0x77, 0x26, 0x61},
    {0x90, 0x01, 0x50, 0x98, 0x3C, 0xD2, 0x4F, 0xB0, 0xD6, 0x96, 0x3F, 0x7D, 0x28, 0xE1, 0x7F, 0x72},
    {0xF9, 0x6B, 0x69, 0x7D, 0x7C, 0xB7, 0x93, 0x8D, 0x52, 0x5A, 0x2F, 0x31, 0xAA, 0xF1, 0x61, 0xD0},
    {0xC3, 0xFC, 0xD3, 0xD7, 0x61, 0x92, 0xE4, 0x00, 0x7D, 0xFB, 0x49, 0x6C, 0xCA, 0x67, 0xE1, 0x3B},
    {0xD1, 0x74, 0xAB, 0x98, 0xD2, 0x77, 0xD9, 0xF5, 0xA5, 0x61, 0x1C, 0x2C, 0x9F, 0x41, 0x9D, 0x9F},
    {0x57, 0xED, 0xF4, 0xA2, 0x2B, 0xE3, 0xC9, 0x55, 0xAC, 0x49, 0xDA, 0x2E, 0x21, 0x07, 0xB6, 0x7A},
};

static_assert(std::extent_v<decltype(md5_test_sum), 0> == kon::array_size(md5_test_buf));

TEST_CASE("md5", "[md5]") {
    for (uint32_t i = 0; i < kon::array_size(md5_test_buf); i++) {
        REQUIRE(md5_test_helper(md5_test_buf[i], md5_test_sum[i]));
    }
}