#include <kon/base16.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("base16_decode", "[base16]") {
    using namespace std::literals;

    {
        static const std::uint8_t data_check[2] = {0x12, 0x34};
        std::string_view str{"1234"sv};
        std::uint8_t data[2];
        std::size_t pos = kon::base16_decode(str.data(), str.size(), data, sizeof(data));

        REQUIRE(pos == 4);
        REQUIRE(memcmp(data_check, data, pos / 2) == 0);
    }

    {
        static const std::uint8_t data_check[2] = {0xCD, 0xEF};
        std::string_view str{"CDEFGH"sv};
        std::uint8_t data[3];
        std::size_t pos = kon::base16_decode(str.data(), str.size(), data, sizeof(data));

        REQUIRE(pos == 4);
        REQUIRE(memcmp(data_check, data, pos / 2) == 0);
    }

    {
        static const std::uint8_t data_check[2] = {0x12, 0x34};
        std::string_view str{"1234Q"sv};
        std::uint8_t data[2];
        std::size_t pos = kon::base16_decode(str.data(), str.size(), data, sizeof(data));

        REQUIRE(pos == 4);
        REQUIRE(memcmp(data_check, data, pos / 2) == 0);
    }

    {
        std::string_view str{"12345"sv};
        std::uint8_t data[3];
        std::size_t pos = kon::base16_decode(str.data(), str.size(), data, sizeof(data));
        REQUIRE(pos == 0);
    }

    {
        std::string_view str{"123456"sv};
        std::uint8_t data[2];
        std::size_t pos = kon::base16_decode(str.data(), str.size(), data, sizeof(data));
        REQUIRE(pos == 0);
    }
}