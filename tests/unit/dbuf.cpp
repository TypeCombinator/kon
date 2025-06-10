#include <catch2/catch_test_macros.hpp>
#include <kon/dbuf.hpp>

struct payload_head {
    uint16_t magic;
    uint16_t length;
};

static const std::uint8_t frame_check[12] =
    {0x34, 0x12, 0x67, 0x45, 0x78, 0x56, 0x34, 0x12, 0x21, 0x43, 0x65, 0x87};

TEST_CASE("append", "[dbuf]") {
    kon::dbuf buf;
    std::uint8_t frame[12] = {};


    buf.init(frame, 0, 0, sizeof(frame));

    auto head = buf.append<payload_head>();
    REQUIRE(head != nullptr);

    head->magic = 0x1234;
    head->length = 0x4567;

    auto element = buf.append<std::uint32_t[]>(2);
    REQUIRE(head != nullptr);

    element[0] = 0x12345678;
    element[1] = 0x87654321;

    CHECK(buf.data_length() == sizeof(frame));
    CHECK(std::memcmp(frame, frame_check, sizeof(frame)) == 0);
}

TEST_CASE("append_rest", "[dbuf]") {
    kon::dbuf buf;
    std::uint8_t frame[12] = {};
    buf.init(frame, 0, 4, sizeof(frame));

    uint32_t data_size;
    uint8_t *data = buf.append_rest_begin(data_size);
    REQUIRE(data_size == 8);
    std::memcpy(data, "123456", 6);
    data_size = 6;
    buf.append(data_size);

    data = buf.append_rest_begin(data_size);
    REQUIRE(data_size == 2);
}

TEST_CASE("prepend", "[dbuf]") {
    kon::dbuf buf;
    std::uint8_t frame[12] = {};
    buf.init(frame, 0, sizeof(payload_head), sizeof(frame));

    auto head = buf.prepend<payload_head>();
    REQUIRE(head != nullptr);

    head->magic = 0x1234;
    head->length = 0x4567;

    auto element = buf.append<std::uint32_t[]>(2);
    REQUIRE(head != nullptr);

    element[0] = 0x12345678;
    element[1] = 0x87654321;

    CHECK(buf.data_length() == sizeof(frame));
    CHECK(std::memcmp(frame, frame_check, sizeof(frame)) == 0);
}

TEST_CASE("read", "[dbuf]") {
    kon::dbuf buf;

    buf.init(const_cast<uint8_t *>(frame_check), 0, 0, sizeof(frame_check));

    buf.reset(0, sizeof(frame_check));

    auto head = buf.read<payload_head>();
    REQUIRE(head != nullptr);

    CHECK(head->magic == 0x1234);
    CHECK(head->length == 0x4567);

    auto element = buf.read<std::uint32_t[]>(2);
    REQUIRE(element != nullptr);

    CHECK(element[0] == 0x12345678);
    CHECK(element[1] == 0x87654321);

    CHECK(buf.data_length() == 0);
}
