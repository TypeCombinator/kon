#include <catch2/catch_test_macros.hpp>
#include <kon/conv.hpp>

template <typename T>
static std::size_t rstring10_to_uint_helper(const std::string_view &str, T &result) noexcept {
    return kon::rstring10_to_uint(str.data(), str.size(), result);
}

TEST_CASE("rstring10_to_uint", "[conv]") {
    SECTION("uint8_t") {
        uint8_t result;

        REQUIRE(rstring10_to_uint_helper("00", result) == 2);
        REQUIRE(result == 0);

        REQUIRE(rstring10_to_uint_helper("01a", result) == 2);
        REQUIRE(result == 1);

        REQUIRE(rstring10_to_uint_helper("123", result) == 3);
        REQUIRE(result == 123);

        REQUIRE(rstring10_to_uint_helper("255", result) == 3);
        REQUIRE(result == 255);

        REQUIRE(rstring10_to_uint_helper("255a", result) == 3);
        REQUIRE(result == 255);

        REQUIRE(rstring10_to_uint_helper("", result) == 0);
        REQUIRE(rstring10_to_uint_helper("a", result) == 0);
        REQUIRE(rstring10_to_uint_helper("266", result) == 0);
        REQUIRE(rstring10_to_uint_helper("256", result) == 0);
        REQUIRE(rstring10_to_uint_helper("2550", result) == 0);
    }

    SECTION("uint16_t") {
        uint16_t result;

        REQUIRE(rstring10_to_uint_helper("00", result) == 2);
        REQUIRE(result == 0);

        REQUIRE(rstring10_to_uint_helper("01a", result) == 2);
        REQUIRE(result == 1);

        REQUIRE(rstring10_to_uint_helper("3333", result) == 4);
        REQUIRE(result == 3333);

        REQUIRE(rstring10_to_uint_helper("65535", result) == 5);
        REQUIRE(result == 65535);

        REQUIRE(rstring10_to_uint_helper("65535a", result) == 5);
        REQUIRE(result == 65535);

        REQUIRE(rstring10_to_uint_helper("", result) == 0);
        REQUIRE(rstring10_to_uint_helper("a", result) == 0);
        REQUIRE(rstring10_to_uint_helper("65546", result) == 0);
        REQUIRE(rstring10_to_uint_helper("65536", result) == 0);
        REQUIRE(rstring10_to_uint_helper("655350", result) == 0);
    }

    SECTION("uint32_t") {
        uint32_t result;

        REQUIRE(rstring10_to_uint_helper("00", result) == 2);
        REQUIRE(result == 0);

        REQUIRE(rstring10_to_uint_helper("01a", result) == 2);
        REQUIRE(result == 1);

        REQUIRE(rstring10_to_uint_helper("33333333", result) == 8);
        REQUIRE(result == 33333333);

        REQUIRE(rstring10_to_uint_helper("4294967295", result) == 10);
        REQUIRE(result == 4294967295);

        REQUIRE(rstring10_to_uint_helper("4294967295a", result) == 10);
        REQUIRE(result == 4294967295);

        REQUIRE(rstring10_to_uint_helper("", result) == 0);
        REQUIRE(rstring10_to_uint_helper("a", result) == 0);
        REQUIRE(rstring10_to_uint_helper("4294967306", result) == 0);
        REQUIRE(rstring10_to_uint_helper("4294967296", result) == 0);
        REQUIRE(rstring10_to_uint_helper("42949672950", result) == 0);
    }

    SECTION("uint64_t") {
        uint64_t result;

        REQUIRE(rstring10_to_uint_helper("00", result) == 2);
        REQUIRE(result == 0);

        REQUIRE(rstring10_to_uint_helper("01a", result) == 2);
        REQUIRE(result == 1);

        REQUIRE(rstring10_to_uint_helper("333333333333333", result) == 15);
        REQUIRE(result == 333333333333333ull);

        REQUIRE(rstring10_to_uint_helper("18446744073709551615", result) == 20);
        REQUIRE(result == 18446744073709551615ull);

        REQUIRE(rstring10_to_uint_helper("18446744073709551615a", result) == 20);
        REQUIRE(result == 18446744073709551615ull);

        REQUIRE(rstring10_to_uint_helper("", result) == 0);
        REQUIRE(rstring10_to_uint_helper("a", result) == 0);
        REQUIRE(rstring10_to_uint_helper("18446744073709551625", result) == 0);
        REQUIRE(rstring10_to_uint_helper("18446744073709551616", result) == 0);
        REQUIRE(rstring10_to_uint_helper("184467440737095516150", result) == 0);
    }
}

template <typename T>
static std::size_t rstring16_to_uint_helper(const std::string_view &str, T &result) noexcept {
    return kon::rstring16_to_uint(str.data(), str.size(), result);
}

TEST_CASE("rstring16_to_uint", "[conv]") {
    SECTION("uint8_t") {
        uint8_t result;

        REQUIRE(rstring16_to_uint_helper("00", result) == 2);
        REQUIRE(result == 0);

        REQUIRE(rstring16_to_uint_helper("F", result) == 1);
        REQUIRE(result == 0xF);

        REQUIRE(rstring16_to_uint_helper("FF", result) == 2);
        REQUIRE(result == 0xFF);

        REQUIRE(rstring16_to_uint_helper("FFx", result) == 2);
        REQUIRE(result == 0xFF);

        REQUIRE(rstring10_to_uint_helper("", result) == 0);
        REQUIRE(rstring10_to_uint_helper("x", result) == 0);
        REQUIRE(rstring10_to_uint_helper("FF0", result) == 0);
    }

    SECTION("uint16_t") {
        uint16_t result;

        REQUIRE(rstring16_to_uint_helper("00", result) == 2);
        REQUIRE(result == 0);

        REQUIRE(rstring16_to_uint_helper("FFF", result) == 3);
        REQUIRE(result == 0xFFF);

        REQUIRE(rstring16_to_uint_helper("FFFF", result) == 4);
        REQUIRE(result == 0xFFFF);

        REQUIRE(rstring16_to_uint_helper("FFFFx", result) == 4);
        REQUIRE(result == 0xFFFF);

        REQUIRE(rstring10_to_uint_helper("", result) == 0);
        REQUIRE(rstring10_to_uint_helper("x", result) == 0);
        REQUIRE(rstring10_to_uint_helper("FFFF0", result) == 0);
    }

    SECTION("uint32_t") {
        uint32_t result;

        REQUIRE(rstring16_to_uint_helper("00", result) == 2);
        REQUIRE(result == 0);

        REQUIRE(rstring16_to_uint_helper("FFFFF", result) == 5);
        REQUIRE(result == 0xFFFFF);

        REQUIRE(rstring16_to_uint_helper("FFFFFFFF", result) == 8);
        REQUIRE(result == 0xFFFFFFFF);

        REQUIRE(rstring16_to_uint_helper("FFFFFFFFx", result) == 8);
        REQUIRE(result == 0xFFFFFFFF);

        REQUIRE(rstring10_to_uint_helper("", result) == 0);
        REQUIRE(rstring10_to_uint_helper("x", result) == 0);
        REQUIRE(rstring10_to_uint_helper("FFFFFFFF0", result) == 0);
    }

    SECTION("uint32_t") {
        uint32_t result;

        REQUIRE(rstring16_to_uint_helper("00", result) == 2);
        REQUIRE(result == 0);

        REQUIRE(rstring16_to_uint_helper("FFFFF", result) == 5);
        REQUIRE(result == 0xFFFFF);

        REQUIRE(rstring16_to_uint_helper("FFFFFFFF", result) == 8);
        REQUIRE(result == 0xFFFFFFFF);

        REQUIRE(rstring16_to_uint_helper("FFFFFFFFx", result) == 8);
        REQUIRE(result == 0xFFFFFFFF);

        REQUIRE(rstring10_to_uint_helper("", result) == 0);
        REQUIRE(rstring10_to_uint_helper("x", result) == 0);
        REQUIRE(rstring10_to_uint_helper("FFFFFFFF0", result) == 0);
    }

    SECTION("uint64_t") {
        uint64_t result;

        REQUIRE(rstring16_to_uint_helper("00", result) == 2);
        REQUIRE(result == 0);

        REQUIRE(rstring16_to_uint_helper("FFFFFFFFFF", result) == 10);
        REQUIRE(result == 0xFFFFFFFFFF);

        REQUIRE(rstring16_to_uint_helper("FFFFFFFFFFFFFFFF", result) == 16);
        REQUIRE(result == 0xFFFFFFFFFFFFFFFF);

        REQUIRE(rstring16_to_uint_helper("FFFFFFFFFFFFFFFFx", result) == 16);
        REQUIRE(result == 0xFFFFFFFFFFFFFFFF);

        REQUIRE(rstring10_to_uint_helper("", result) == 0);
        REQUIRE(rstring10_to_uint_helper("x", result) == 0);
        REQUIRE(rstring10_to_uint_helper("FFFFFFFFFFFFFFFF0", result) == 0);
    }
}

template <typename T>
static std::size_t string10_to_uint_helper(const std::string_view &str, T &result) noexcept {
    return kon::string10_to_uint(str.data(), str.size(), result);
}

TEST_CASE("string10_to_uint", "[conv]") {
    uint8_t result;

    REQUIRE(string10_to_uint_helper("00", result) == 2);
    REQUIRE(result == 0);

    REQUIRE(string10_to_uint_helper("+00", result) == 3);
    REQUIRE(result == 0);

    REQUIRE(string10_to_uint_helper("+12", result) == 3);
    REQUIRE(result == 12);

    REQUIRE(string10_to_uint_helper("+", result) == 0);
}

template <typename T>
static std::size_t string16_to_uint_helper(const std::string_view &str, T &result) noexcept {
    return kon::string16_to_uint(str.data(), str.size(), result);
}

TEST_CASE("string16_to_uint", "[conv]") {
    uint8_t result;

    REQUIRE(string16_to_uint_helper("0x0", result) == 3);
    REQUIRE(result == 0);

    REQUIRE(string16_to_uint_helper("+0x0", result) == 4);
    REQUIRE(result == 0);

    REQUIRE(string16_to_uint_helper("+0x12", result) == 5);
    REQUIRE(result == 0x12);

    REQUIRE(string16_to_uint_helper("+0", result) == 0);
    REQUIRE(string16_to_uint_helper("+0x", result) == 0);
}

template <typename T>
static std::size_t string10_to_int_helper(const std::string_view &str, T &result) noexcept {
    return kon::string10_to_int(str.data(), str.size(), result);
}

TEST_CASE("string10_to_int", "[conv]") {
    int8_t result;

    REQUIRE(string10_to_int_helper("00", result) == 2);
    REQUIRE(result == 0);

    REQUIRE(string10_to_int_helper("-0", result) == 2);
    REQUIRE(result == 0);

    REQUIRE(string10_to_int_helper("+0", result) == 2);
    REQUIRE(result == 0);

    REQUIRE(string10_to_int_helper("+127", result) == 4);
    REQUIRE(result == 127);

    REQUIRE(string10_to_int_helper("127", result) == 3);
    REQUIRE(result == 127);

    REQUIRE(string10_to_int_helper("-128", result) == 4);
    REQUIRE(result == -128);


    REQUIRE(string10_to_int_helper("-", result) == 0);
    REQUIRE(string10_to_int_helper("+", result) == 0);
    REQUIRE(string10_to_int_helper("128", result) == 0);
    REQUIRE(string10_to_int_helper("1270", result) == 0);
    REQUIRE(string10_to_int_helper("-129", result) == 0);
}

template <typename T>
static std::size_t string16_to_int_helper(const std::string_view &str, T &result) noexcept {
    return kon::string16_to_int(str.data(), str.size(), result);
}

TEST_CASE("string16_to_int", "[conv]") {
    int8_t result;

    REQUIRE(string16_to_int_helper("0x0", result) == 3);
    REQUIRE(result == 0);

    REQUIRE(string16_to_int_helper("-0x0", result) == 4);
    REQUIRE(result == 0);

    REQUIRE(string16_to_int_helper("+0x0", result) == 4);
    REQUIRE(result == 0);

    REQUIRE(string16_to_int_helper("+0xF", result) == 4);
    REQUIRE(result == 0xF);

    REQUIRE(string16_to_int_helper("-0x80", result) == 5);
    REQUIRE(result == -0x80);

    REQUIRE(string16_to_int_helper("-", result) == 0);
    REQUIRE(string16_to_int_helper("+", result) == 0);
    REQUIRE(string16_to_int_helper("0x", result) == 0);
    REQUIRE(string16_to_int_helper("+0x", result) == 0);
    REQUIRE(string16_to_int_helper("0x80", result) == 0);
    REQUIRE(string16_to_int_helper("-0x81", result) == 0);
    REQUIRE(string16_to_int_helper("-0x800", result) == 0);
}

template <typename T>
static std::size_t string_to_float_helper(const std::string_view &str, T &result) noexcept {
    return kon::string_to_float(str.data(), str.size(), result);
}

TEST_CASE("string_to_float", "[conv]") {
    float result;

    REQUIRE(string_to_float_helper("0", result) == 1);
    REQUIRE(result == 0.0);

    REQUIRE(string_to_float_helper("0.1", result) == 3);
    REQUIRE(result == 0.1f);

    REQUIRE(string_to_float_helper("-0.1", result) == 4);
    REQUIRE(result == -0.1f);

    REQUIRE(string_to_float_helper("-3e2", result) == 4);
    REQUIRE(result == -3e2f);
}

template <typename T>
static std::size_t string_to_uint_helper(const std::string_view &str, T &result) noexcept {
    return kon::string_to_uint(str.data(), str.size(), result);
}

TEST_CASE("string_to_uint", "[conv]") {
    uint8_t result;

    REQUIRE(string_to_uint_helper("0", result) == 1);
    REQUIRE(result == 0);

    REQUIRE(string_to_uint_helper("+0", result) == 2);
    REQUIRE(result == 0);

    REQUIRE(string_to_uint_helper("0x0", result) == 3);
    REQUIRE(result == 0);

    REQUIRE(string_to_uint_helper("+0x0", result) == 4);
    REQUIRE(result == 0);

    REQUIRE(string_to_uint_helper("+0x12", result) == 5);
    REQUIRE(result == 0x12);

    REQUIRE(string_to_uint_helper("+0xFF", result) == 5);
    REQUIRE(result == 0xFF);

    REQUIRE(string_to_uint_helper("-0", result) == 0);
    REQUIRE(string_to_uint_helper("-0x", result) == 0);
    REQUIRE(string_to_uint_helper("+0x", result) == 0);
    REQUIRE(string_to_uint_helper("0x", result) == 0);
}

template <typename T>
static std::size_t string_to_int_helper(const std::string_view &str, T &result) noexcept {
    return kon::string_to_int(str.data(), str.size(), result);
}

TEST_CASE("string_to_int", "[conv]") {
    int8_t result;

    REQUIRE(string_to_int_helper("0", result) == 1);
    REQUIRE(result == 0);

    REQUIRE(string_to_int_helper("+0", result) == 2);
    REQUIRE(result == 0);

    REQUIRE(string_to_int_helper("-0", result) == 2);
    REQUIRE(result == 0);

    REQUIRE(string_to_int_helper("0x0", result) == 3);
    REQUIRE(result == 0);

    REQUIRE(string_to_int_helper("+0x0", result) == 4);
    REQUIRE(result == 0);

    REQUIRE(string_to_int_helper("-0x0", result) == 4);
    REQUIRE(result == 0);

    REQUIRE(string_to_int_helper("127", result) == 3);
    REQUIRE(result == 127);

    REQUIRE(string_to_int_helper("+127", result) == 4);
    REQUIRE(result == 127);

    REQUIRE(string_to_int_helper("0x7F", result) == 4);
    REQUIRE(result == 0x7F);

    REQUIRE(string_to_int_helper("+0x7F", result) == 5);
    REQUIRE(result == 0x7F);

    REQUIRE(string_to_int_helper("-128", result) == 4);
    REQUIRE(result == -128);

    REQUIRE(string_to_int_helper("-0x80", result) == 5);
    REQUIRE(result == -0x80);

    REQUIRE(string_to_int_helper("128", result) == 0);
    REQUIRE(string_to_int_helper("0x", result) == 0);
    REQUIRE(string_to_int_helper("0x80", result) == 0);
    REQUIRE(string_to_int_helper("+0x80", result) == 0);
    REQUIRE(string_to_int_helper("-129", result) == 0);
    REQUIRE(string_to_int_helper("-0x81", result) == 0);
}