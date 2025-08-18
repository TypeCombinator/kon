#include <kon/bit.hpp>
#include <random>
#include <catch2/catch_template_test_macros.hpp>

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

TEST_CASE("countl_zero", "[bit]") {
    SECTION("uint8_t") {
        uint8_t value = 1;
        REQUIRE(kon::countl_zero(value) == 7);
        value = 0;
        REQUIRE(kon::countl_zero(value) == 8);
        value = ~value;
        REQUIRE(kon::countl_zero(value) == 0);
        value = 0x7F;
        REQUIRE(kon::countl_zero(value) == 1);
        value = 0x80;
        for (uint32_t i = 0; i < 8; i++) {
            REQUIRE(kon::countl_zero(value) == i);
            value >>= 1;
        }
    }
    SECTION("uint16_t") {
        uint16_t value = 1;
        REQUIRE(kon::countl_zero(value) == 15);
        value = 0;
        REQUIRE(kon::countl_zero(value) == 16);
        value = ~value;
        REQUIRE(kon::countl_zero(value) == 0);
        value = 0x7FFF;
        REQUIRE(kon::countl_zero(value) == 1);
        value = 0x8000;
        for (uint32_t i = 0; i < 16; i++) {
            REQUIRE(kon::countl_zero(value) == i);
            value >>= 1;
        }
    }
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

TEST_CASE("count_one", "[bit]") {
    SECTION("uint32_t") {
        uint32_t value = 1;
        REQUIRE(kon::popcount(value) == 1);
        value = 0;
        REQUIRE(kon::popcount(value) == 0);
        value = ~value;
        REQUIRE(kon::popcount(value) == 32);
        value = 0xEF0;
        REQUIRE(kon::popcount(value) == 7);
        value = 0;
        for (uint32_t i = 0; i <= 32; i++) {
            REQUIRE(kon::popcount(value) == i);
            REQUIRE(kon::popcount(~value) == 32 - i);
            value |= (static_cast<uint32_t>(1) << i);
        }
    }
    SECTION("uint32_t") {
        uint64_t value = 1;
        REQUIRE(kon::popcount(value) == 1);
        value = 0;
        REQUIRE(kon::popcount(value) == 0);
        value = ~value;
        REQUIRE(kon::popcount(value) == 64);
        value = 0xEF0;
        REQUIRE(kon::popcount(~value) == (64 - 7));
        value = 0;
        for (uint32_t i = 0; i <= 64; i++) {
            REQUIRE(kon::popcount(value) == i);
            REQUIRE(kon::popcount(~value) == 64 - i);
            value |= (static_cast<uint64_t>(1) << i);
        }
    }
}

TEMPLATE_TEST_CASE(
    "bit_for_each",
    "[bitset]",
    std::uint32_t,
    std::uint64_t,
    std::int32_t,
    std::int64_t) {
    constexpr std::uint32_t bit_number = sizeof(TestType) * 8;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<TestType> dist(std::numeric_limits<TestType>::min());

    uint8_t expect_out[bit_number], out[bit_number];
    uint8_t expect_count, count;

    for (std::uint32_t times = 10; times > 0; times--) {
        TestType data = dist(gen);

        auto t_data = data;
        expect_count = 0;
        for (std::uint32_t i = 0; i < bit_number; i++) {
            if (t_data & 1) {
                expect_out[expect_count] = i;
                expect_count++;
            }
            t_data >>= 1;
        }

        count = 0;
        kon::bit_for_each(data, [&out, &count](uint32_t i) {
            out[count] = i;
            count++;
        });

        REQUIRE(count == expect_count);
        for (std::uint32_t i = 0; i < count; i++) {
            REQUIRE(out[i] == expect_out[i]);
        }
    }
}

TEMPLATE_TEST_CASE(
    "bit_reverse_for_each",
    "[bitset]",
    std::uint32_t,
    std::uint64_t,
    std::int32_t,
    std::int64_t) {
    constexpr std::uint32_t bit_number = sizeof(TestType) * 8;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<TestType> dist(std::numeric_limits<TestType>::min());

    uint8_t expect_out[bit_number], out[bit_number];
    uint8_t expect_count, count;

    for (std::uint32_t times = 10; times > 0; times--) {
        TestType data = dist(gen);

        auto t_data = data;
        expect_count = 0;
        for (std::uint32_t i = 0; i < bit_number; i++) {
            if (t_data & (static_cast<TestType>(1) << (bit_number - 1))) {
                expect_out[expect_count] = i;
                expect_count++;
            }
            t_data <<= 1;
        }

        count = 0;
        kon::bit_reverse_for_each(data, [&out, &count](uint32_t i) {
            out[count] = i;
            count++;
        });

        REQUIRE(count == expect_count);
        for (std::uint32_t i = 0; i < count; i++) {
            REQUIRE(out[i] == expect_out[i]);
        }
    }
}

TEMPLATE_TEST_CASE(
    "bit_iterator",
    "[bitset]",
    std::uint32_t,
    std::uint64_t,
    std::int32_t,
    std::int64_t) {
    constexpr std::uint32_t bit_number = sizeof(TestType) * 8;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<TestType> dist(std::numeric_limits<TestType>::min());

    uint8_t expect_out[bit_number], out[bit_number];
    uint8_t expect_count, count;

    for (std::uint32_t times = 10; times > 0; times--) {
        TestType data = dist(gen);

        auto t_data = data;
        expect_count = 0;
        for (std::uint32_t i = 0; i < bit_number; i++) {
            if (t_data & 1) {
                expect_out[expect_count] = i;
                expect_count++;
            }
            t_data >>= 1;
        }

        count = 0;
        std::uint32_t i;
        for (kon::bit_iterator it{data}; it.next(i);) {
            out[count] = i;
            count++;
        }

        REQUIRE(count == expect_count);
        for (std::uint32_t i = 0; i < count; i++) {
            REQUIRE(out[i] == expect_out[i]);
        }
    }
}

TEMPLATE_TEST_CASE(
    "bit_reverse_iterator",
    "[bitset]",
    std::uint32_t,
    std::uint64_t,
    std::int32_t,
    std::int64_t) {
    constexpr std::uint32_t bit_number = sizeof(TestType) * 8;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<TestType> dist(std::numeric_limits<TestType>::min());

    uint8_t expect_out[bit_number], out[bit_number];
    uint8_t expect_count, count;

    for (std::uint32_t times = 10; times > 0; times--) {
        TestType data = dist(gen);

        auto t_data = data;
        expect_count = 0;
        for (std::uint32_t i = 0; i < bit_number; i++) {
            if (t_data & (static_cast<TestType>(1) << (bit_number - 1))) {
                expect_out[expect_count] = i;
                expect_count++;
            }
            t_data <<= 1;
        }

        count = 0;
        std::uint32_t i;
        for (kon::bit_reverse_iterator it{data}; it.next(i);) {
            out[count] = i;
            count++;
        }

        REQUIRE(count == expect_count);
        for (std::uint32_t i = 0; i < count; i++) {
            REQUIRE(out[i] == expect_out[i]);
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

TEST_CASE("bit_imask", "[bit]") {
    STATIC_REQUIRE(kon::bit_imask<uint8_t>(7, 0) == 0b0000'0000);
    STATIC_REQUIRE(kon::bit_imask<uint8_t>(7, 1) == 0b0000'0001);
    STATIC_REQUIRE(kon::bit_imask<uint8_t>(6, 0) == 0b1000'0000);
    STATIC_REQUIRE(kon::bit_imask<uint8_t>(6, 1) == 0b1000'0001);
    STATIC_REQUIRE(kon::bit_imask<uint8_t>(4, 3) == 0b1110'0111);
}

TEST_CASE("lsb_mask", "[bit]") {
    STATIC_REQUIRE(kon::lsb_mask<uint8_t>(7) == 0b1111'1111);
    STATIC_REQUIRE(kon::lsb_mask<uint8_t>(6) == 0b0111'1111);
    STATIC_REQUIRE(kon::lsb_mask<uint8_t>(3) == 0b0000'1111);
    STATIC_REQUIRE(kon::lsb_mask<uint8_t>(1) == 0b0000'0011);
    STATIC_REQUIRE(kon::lsb_mask<uint8_t>(0) == 0b0000'0001);
}

TEST_CASE("msb_mask", "[bit]") {
    STATIC_REQUIRE(kon::msb_mask<uint8_t>(7) == 0b1000'0000);
    STATIC_REQUIRE(kon::msb_mask<uint8_t>(6) == 0b1100'0000);
    STATIC_REQUIRE(kon::msb_mask<uint8_t>(3) == 0b1111'1000);
    STATIC_REQUIRE(kon::msb_mask<uint8_t>(1) == 0b1111'1110);
    STATIC_REQUIRE(kon::msb_mask<uint8_t>(0) == 0b1111'1111);
}

TEST_CASE("lsb_imask", "[bit]") {
    STATIC_REQUIRE(kon::lsb_imask<uint8_t>(7) == 0b0000'0000);
    STATIC_REQUIRE(kon::lsb_imask<uint8_t>(6) == 0b1000'0000);
    STATIC_REQUIRE(kon::lsb_imask<uint8_t>(3) == 0b1111'0000);
    STATIC_REQUIRE(kon::lsb_imask<uint8_t>(1) == 0b1111'1100);
    STATIC_REQUIRE(kon::lsb_imask<uint8_t>(0) == 0b1111'1110);
}

TEST_CASE("msb_imask", "[bit]") {
    STATIC_REQUIRE(kon::msb_imask<uint8_t>(7) == 0b0111'1111);
    STATIC_REQUIRE(kon::msb_imask<uint8_t>(6) == 0b0011'1111);
    STATIC_REQUIRE(kon::msb_imask<uint8_t>(3) == 0b0000'0111);
    STATIC_REQUIRE(kon::msb_imask<uint8_t>(1) == 0b0000'0001);
    STATIC_REQUIRE(kon::msb_imask<uint8_t>(0) == 0b0000'0000);
}