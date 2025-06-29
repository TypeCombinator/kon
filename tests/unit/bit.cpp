#include <kon/bit.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("byteswap", "[bit]") {
    SECTION("uint16_t") {
        uint16_t value = 0x1234;
        REQUIRE(kon::byteswap(value) == 0x3412);
    }
    SECTION("uint32_t") {
        uint32_t value = 0x12345678;
        REQUIRE(kon::byteswap(value) == 0x78563412);
    }
    SECTION("uint64_t") {
        uint32_t value = 0x12345678;
        REQUIRE(kon::byteswap(value) == 0x78563412);
    }
}

TEST_CASE("count_zero", "[bit]") {
    SECTION("uint32_t") {
        uint32_t value = 1;
        REQUIRE(kon::countl_zero(value) == 31);
        value = 0;
        REQUIRE(kon::countl_zero(value) == 32);
        value = ~value;
        REQUIRE(kon::countl_zero(value) == 0);
        value = 0x7FFFFFFF;
        REQUIRE(kon::countl_zero(value) == 1);
        for (uint32_t i = 0; i < 32; i++) {
            REQUIRE(kon::countl_zero(0x80000000u >> i) == i);
        }
    }
    SECTION("uint64_t") {
        uint64_t value = 1;
        REQUIRE(kon::countl_zero(value) == 63);
        value = 0;
        REQUIRE(kon::countl_zero(value) == 64);
        value = ~value;
        REQUIRE(kon::countl_zero(value) == 0);
        value = 1ull << 63;
        for (uint32_t i = 0; i < 64; i++) {
            REQUIRE(kon::countl_zero(value >> i) == i);
        }
    }
}

TEST_CASE("countr_zero", "[bit]") {
    SECTION("uint32_t") {
        uint32_t value = 1;
        REQUIRE(kon::countr_zero(value) == 0);
        value = 0;
        REQUIRE(kon::countr_zero(value) == 32);
        value = ~value;
        REQUIRE(kon::countr_zero(value) == 0);
        value = 0xE;
        REQUIRE(kon::countr_zero(value) == 1);
        value = 1;
        for (uint32_t i = 0; i < 32; i++) {
            REQUIRE(kon::countr_zero(value << i) == i);
        }
    }
    SECTION("uint64_t") {
        uint64_t value = 1;
        REQUIRE(kon::countr_zero(value) == 0);
        value = 0;
        REQUIRE(kon::countr_zero(value) == 64);
        value = ~value;
        REQUIRE(kon::countr_zero(value) == 0);
        value = 0xE;
        REQUIRE(kon::countr_zero(value) == 1);
        value = 1;
        for (uint32_t i = 0; i < 64; i++) {
            REQUIRE(kon::countr_zero(value << i) == i);
        }
    }
}

TEST_CASE("bit_mask", "[bit]") {
    STATIC_REQUIRE(kon::bit_mask<uint8_t>(7, 0) == 0b1111'1111);
    STATIC_REQUIRE(kon::bit_mask<uint8_t>(7, 1) == 0b1111'1110);
    STATIC_REQUIRE(kon::bit_mask<uint8_t>(6, 0) == 0b0111'1111);
    STATIC_REQUIRE(kon::bit_mask<uint8_t>(6, 1) == 0b0111'1110);
    STATIC_REQUIRE(kon::bit_mask<uint8_t>(4, 3) == 0b0001'1000);
}