#include <kon/bio.hpp>
#include <catch2/catch_test_macros.hpp>

namespace kon_bio_test {
struct foo {
    uint32_t x;
    uint16_t y;
    uint16_t z;
};
} // namespace kon_bio_test

TEST_CASE("little_endian", "[bio]") {
    uint8_t idata[sizeof(kon_bio_test::foo)] = {0x78, 0x56, 0x34, 0x12, 0x34, 0x12, 0x78, 0x56};
    SECTION("read") {
        kon::bio<kon_bio_test::foo> view{idata};
        REQUIRE(KON_BIO_READ(view, x) == 0x12345678);
        REQUIRE(KON_BIO_READ(view, y) == 0x1234);
        REQUIRE(KON_BIO_READ(view, z) == 0x5678);
    }
    SECTION("read_to") {
        kon::bio<kon_bio_test::foo> view{idata};
        uint32_t t_x;
        uint16_t t_y;
        uint16_t t_z;
        KON_BIO_READ_TO(view, t_x, x);
        KON_BIO_READ_TO(view, t_y, y);
        KON_BIO_READ_TO(view, t_z, z);

        REQUIRE(t_x == 0x12345678);
        REQUIRE(t_y == 0x1234);
        REQUIRE(t_z == 0x5678);
    }

    SECTION("write") {
        std::uint8_t odata[sizeof(kon_bio_test::foo)];
        kon::bio<kon_bio_test::foo> view{odata};

        KON_BIO_WRITE(view, 0x12345678, x);
        KON_BIO_WRITE(view, 0x1234, y);
        KON_BIO_WRITE(view, 0x5678, z);

        REQUIRE(std::memcmp(idata, odata, sizeof(idata)) == 0);
    }
}

TEST_CASE("big_endian", "[bio]") {
    uint8_t idata[sizeof(kon_bio_test::foo)] = {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78};
    SECTION("read") {
        kon::bio<kon_bio_test::foo, true> view{idata};
        REQUIRE(KON_BIO_READ(view, x) == 0x12345678);
        REQUIRE(KON_BIO_READ(view, y) == 0x1234);
        REQUIRE(KON_BIO_READ(view, z) == 0x5678);
    }
    SECTION("read_to") {
        kon::bio<kon_bio_test::foo, true> view{idata};
        uint32_t t_x;
        uint16_t t_y;
        uint16_t t_z;
        KON_BIO_READ_TO(view, t_x, x);
        KON_BIO_READ_TO(view, t_y, y);
        KON_BIO_READ_TO(view, t_z, z);

        REQUIRE(t_x == 0x12345678);
        REQUIRE(t_y == 0x1234);
        REQUIRE(t_z == 0x5678);
    }

    SECTION("write") {
        std::uint8_t odata[sizeof(kon_bio_test::foo)];
        kon::bio<kon_bio_test::foo, true> view{odata};

        KON_BIO_WRITE(view, 0x12345678, x);
        KON_BIO_WRITE(view, 0x1234, y);
        KON_BIO_WRITE(view, 0x5678, z);

        REQUIRE(std::memcmp(idata, odata, sizeof(idata)) == 0);
    }
}