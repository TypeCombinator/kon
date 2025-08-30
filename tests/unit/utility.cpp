#include <kon/utility.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("utility", "[utility]") {
    STATIC_REQUIRE(kon::max<std::size_t, 3, 7, 2, 6>() == 7);

    STATIC_REQUIRE(std::is_same_v<kon::narrow_uint_t<255>, std::uint8_t>);
    STATIC_REQUIRE(std::is_same_v<kon::narrow_uint_t<256>, std::uint16_t>);
    STATIC_REQUIRE(std::is_same_v<kon::narrow_uint_t<65536>, std::uint32_t>);
    STATIC_REQUIRE(std::is_same_v<kon::narrow_uint_t<4294967296ull>, std::uint64_t>);
}