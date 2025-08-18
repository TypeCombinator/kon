#include <kon/bitset.hpp>
#include <random>
#include <catch2/catch_template_test_macros.hpp>

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

TEST_CASE("run time bitset", "[bitset]") {
    kon::bitset<23, uint8_t> bitmap;
    kon::bitset<24, uint8_t> aligned_bitmap;

    REQUIRE(bitmap.element_number == 3);
    REQUIRE(bitmap.element_bit_width == 8);
    bitmap.set().flip(23);
    REQUIRE(bitmap.data[0] == 0xFF);
    REQUIRE(bitmap.data[1] == 0xFF);
    REQUIRE(bitmap.data[2] == 0x7F);
    REQUIRE(bitmap.all());

    bitmap.reset().flip(23);
    REQUIRE(bitmap.data[0] == 0);
    REQUIRE(bitmap.data[1] == 0);
    REQUIRE(bitmap.data[2] == 0x80);
    REQUIRE(bitmap.none());
    REQUIRE(!bitmap.any());

    SECTION("set") {
        bitmap.reset().set(12);
        REQUIRE(bitmap.data[0] == 0);
        REQUIRE(bitmap.data[1] == 0b0001'0000);
        REQUIRE(bitmap.data[2] == 0);

        bitmap.reset().set(14, 11);
        REQUIRE(bitmap.data[0] == 0);
        REQUIRE(bitmap.data[1] == 0b0111'1000);
        REQUIRE(bitmap.data[2] == 0);

        bitmap.reset().set(18, 6);
        REQUIRE(bitmap.data[0] == 0b1100'0000);
        REQUIRE(bitmap.data[1] == 0xFF);
        REQUIRE(bitmap.data[2] == 0b0000'0111);
    }

    SECTION("reset") {
        bitmap.set().reset(12);
        REQUIRE(bitmap.data[0] == 0xFF);
        REQUIRE(bitmap.data[1] == 0b1110'1111);
        REQUIRE(bitmap.data[2] == 0xFF);

        bitmap.set().reset(14, 11);
        REQUIRE(bitmap.data[0] == 0xFF);
        REQUIRE(bitmap.data[1] == 0b1000'0111);
        REQUIRE(bitmap.data[2] == 0xFF);

        bitmap.set().reset(18, 6);
        REQUIRE(bitmap.data[0] == 0b0011'1111);
        REQUIRE(bitmap.data[1] == 0);
        REQUIRE(bitmap.data[2] == 0b1111'1000);
    }

    SECTION("flip") {
        bitmap.set().flip(12);
        REQUIRE(bitmap.data[0] == 0xFF);
        REQUIRE(bitmap.data[1] == 0b1110'1111);
        REQUIRE(bitmap.data[2] == 0xFF);

        bitmap.flip(12);
        REQUIRE(bitmap.data[0] == 0xFF);
        REQUIRE(bitmap.data[1] == 0xFF);
        REQUIRE(bitmap.data[2] == 0xFF);


        bitmap.set().flip(14, 11);
        REQUIRE(bitmap.data[0] == 0xFF);
        REQUIRE(bitmap.data[1] == 0b1000'0111);
        REQUIRE(bitmap.data[2] == 0xFF);

        bitmap.flip(14, 11);
        REQUIRE(bitmap.data[0] == 0xFF);
        REQUIRE(bitmap.data[1] == 0xFF);
        REQUIRE(bitmap.data[2] == 0xFF);


        bitmap.set().flip(18, 6);
        REQUIRE(bitmap.data[0] == 0b0011'1111);
        REQUIRE(bitmap.data[1] == 0);
        REQUIRE(bitmap.data[2] == 0b1111'1000);

        bitmap.flip(18, 6);
        REQUIRE(bitmap.data[0] == 0xFF);
        REQUIRE(bitmap.data[1] == 0xFF);
        REQUIRE(bitmap.data[2] == 0xFF);
    }

    SECTION("count") {
        bitmap.set();
        REQUIRE(bitmap.count() == 23);
        bitmap.reset();
        REQUIRE(bitmap.count() == 0);

        bitmap.flip(18, 6);
        REQUIRE(bitmap.count() == 13);

        bitmap.set().flip(18, 6);
        REQUIRE(bitmap.count() == bitmap.size() - 13);

        aligned_bitmap.set().flip(18, 6);
        REQUIRE(aligned_bitmap.count() == aligned_bitmap.size() - 13);
    }

    kon::bitset<23, uint8_t> bitmap_other;

    SECTION("equal") {
        bitmap.reset().set(23).set(18, 6);
        bitmap_other.reset().flip(18, 6);
        REQUIRE(bitmap == bitmap_other);
        bitmap_other.flip(10);
        REQUIRE(bitmap != bitmap_other);
    }

    SECTION("bitwise not") {
        bitmap.reset();
        bitmap = ~bitmap;
        REQUIRE(bitmap.data[0] == 0xFF);
        REQUIRE(bitmap.data[1] == 0xFF);
        REQUIRE(bitmap.data[2] == 0xFF);
    }

    SECTION("mask") {
        auto mask = kon::bitset<23, uint8_t>::mask(18, 6);
        REQUIRE(mask.data[0] == 0b1100'0000);
        REQUIRE(mask.data[1] == 0xFF);
        REQUIRE(mask.data[2] == 0b0000'0111);

        mask = kon::bitset<23, uint8_t>::mask(11, 9);
        REQUIRE(mask.data[0] == 0);
        REQUIRE(mask.data[1] == 0b0000'1110);
        REQUIRE(mask.data[2] == 0);
    }

    SECTION("imask") {
        auto mask = kon::bitset<23, uint8_t>::imask(18, 6);
        REQUIRE(mask.data[0] == 0b0011'1111);
        REQUIRE(mask.data[1] == 0);
        REQUIRE(mask.data[2] == 0b1111'1000);

        mask = kon::bitset<23, uint8_t>::imask(11, 9);
        REQUIRE(mask.data[0] == 0xFF);
        REQUIRE(mask.data[1] == 0b1111'0001);
        REQUIRE(mask.data[2] == 0xFF);
    }

    SECTION("left shift assign") {
        bitmap = {0x12, 0x34, 0x56};
        bitmap <<= 1;
        REQUIRE(bitmap.data[0] == 0x24);
        REQUIRE(bitmap.data[1] == 0x68);
        REQUIRE(bitmap.data[2] == 0xAC);

        bitmap <<= 7;
        REQUIRE(bitmap.data[0] == 0);
        REQUIRE(bitmap.data[1] == 0x12);
        REQUIRE(bitmap.data[2] == 0x34);

        bitmap = {0x12, 0x34, 0x56};
        bitmap <<= 8;
        REQUIRE(bitmap.data[0] == 0);
        REQUIRE(bitmap.data[1] == 0x12);
        REQUIRE(bitmap.data[2] == 0x34);
    }

    SECTION("right shift assign") {
        bitmap = {0x24, 0x68, 0xAC};
        bitmap >>= 1;
        REQUIRE(bitmap.data[0] == 0x12);
        REQUIRE(bitmap.data[1] == 0x34);
        REQUIRE(bitmap.data[2] == 0x56);

        bitmap >>= 7;
        REQUIRE(bitmap.data[0] == 0x68);
        REQUIRE(bitmap.data[1] == 0xAC);
        REQUIRE(bitmap.data[2] == 0x00);

        bitmap = {0x24, 0x68, 0xAC};
        bitmap >>= 8;
        REQUIRE(bitmap.data[0] == 0x68);
        REQUIRE(bitmap.data[1] == 0xAC);
        REQUIRE(bitmap.data[2] == 0x00);
    }

    SECTION("left shift") {
        kon::bitset<23, uint8_t> bs{0x12, 0x34, 0x56};
        bitmap = (bs << 1);
        REQUIRE(bitmap.data[0] == 0x24);
        REQUIRE(bitmap.data[1] == 0x68);
        REQUIRE(bitmap.data[2] == 0xAC);

        bitmap = (bitmap << 7);
        REQUIRE(bitmap.data[0] == 0);
        REQUIRE(bitmap.data[1] == 0x12);
        REQUIRE(bitmap.data[2] == 0x34);

        bitmap = (bs << 8);
        REQUIRE(bitmap.data[0] == 0);
        REQUIRE(bitmap.data[1] == 0x12);
        REQUIRE(bitmap.data[2] == 0x34);
    }

    SECTION("right shift") {
        kon::bitset<23, uint8_t> bs{0x24, 0x68, 0xAC};
        bitmap = (bs >> 1);
        REQUIRE(bitmap.data[0] == 0x12);
        REQUIRE(bitmap.data[1] == 0x34);
        REQUIRE(bitmap.data[2] == 0x56);

        bitmap = (bitmap >> 7);
        REQUIRE(bitmap.data[0] == 0x68);
        REQUIRE(bitmap.data[1] == 0xAC);
        REQUIRE(bitmap.data[2] == 0x00);

        bitmap = (bs >> 8);
        REQUIRE(bitmap.data[0] == 0x68);
        REQUIRE(bitmap.data[1] == 0xAC);
        REQUIRE(bitmap.data[2] == 0x00);
    }

    SECTION("and assign") {
        kon::bitset<23, uint8_t> bs{0x55, 0xAA, 0x55};
        bitmap = {0xAA, 0x55, 0xAA};

        bitmap &= bs;
        REQUIRE(bitmap.data[0] == 0);
        REQUIRE(bitmap.data[1] == 0);
        REQUIRE(bitmap.data[2] == 0);
    }

    SECTION("or assign") {
        kon::bitset<23, uint8_t> bs{0x55, 0xAA, 0x55};
        bitmap = {0xAA, 0x55, 0xAA};

        bitmap |= bs;
        REQUIRE(bitmap.data[0] == 0xFF);
        REQUIRE(bitmap.data[1] == 0xFF);
        REQUIRE(bitmap.data[2] == 0xFF);
    }

    SECTION("xor assign") {
        kon::bitset<23, uint8_t> bs{0x55, 0xAA, 0x55};
        bitmap = {0xAA, 0x55, 0xAA};

        bitmap ^= bs;
        REQUIRE(bitmap.data[0] == 0xFF);
        REQUIRE(bitmap.data[1] == 0xFF);
        REQUIRE(bitmap.data[2] == 0xFF);

        bitmap ^= bs;
        REQUIRE(bitmap == ~bs);
    }

    SECTION("and") {
        kon::bitset<23, uint8_t> bs{0x55, 0xAA, 0x55};
        bitmap = {0xAA, 0x55, 0xAA};

        bitmap = bs & bitmap;
        REQUIRE(bitmap.data[0] == 0);
        REQUIRE(bitmap.data[1] == 0);
        REQUIRE(bitmap.data[2] == 0);
    }

    SECTION("or") {
        kon::bitset<23, uint8_t> bs{0x55, 0xAA, 0x55};
        bitmap = {0xAA, 0x55, 0xAA};

        bitmap = bs | bitmap;
        REQUIRE(bitmap.data[0] == 0xFF);
        REQUIRE(bitmap.data[1] == 0xFF);
        REQUIRE(bitmap.data[2] == 0xFF);
    }

    SECTION("xor") {
        kon::bitset<23, uint8_t> bs{0x55, 0xAA, 0x55};
        bitmap = {0xAA, 0x55, 0xAA};

        bitmap = bs ^ bitmap;
        REQUIRE(bitmap.data[0] == 0xFF);
        REQUIRE(bitmap.data[1] == 0xFF);
        REQUIRE(bitmap.data[2] == 0xFF);

        bitmap = bs ^ bitmap;
        REQUIRE(bitmap == ~bs);
    }
}