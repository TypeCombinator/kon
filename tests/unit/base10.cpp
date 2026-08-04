#include <kon/base10.hpp>
#include <random>
#include <catch2/catch_test_macros.hpp>
#include <charconv>
#include <limits>

static constexpr std::uint8_t slow_digit10_count(std::uint64_t value) noexcept {
    std::uint8_t count = 0;
    do {
        count++;
        value = value / 10;
    } while (value != 0);
    return count;
}

TEST_CASE("digit10_count", "[base10]") {
    static_assert(kon::base10_count(0u) == 1);
    static_assert(
        kon::base10_count(std::numeric_limits<std::uint32_t>::max())
        == std::numeric_limits<std::uint32_t>::digits10 + 1);
    static_assert(kon::base10_count(0ul) == 1);
    static_assert(
        kon::base10_count(std::numeric_limits<std::uint64_t>::max())
        == std::numeric_limits<std::uint64_t>::digits10 + 1);

    SECTION("uint32_t") {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::uint32_t> dist(0);
        for (std::uint32_t times = 100; times > 0; times--) {
            auto data = dist(gen);
            REQUIRE(kon::base10_count(data) == slow_digit10_count(data));
        }
    }
    SECTION("uint64_t") {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::uint64_t> dist(0);
        for (std::uint32_t times = 100; times > 0; times--) {
            auto data = dist(gen);
            REQUIRE(kon::base10_count(data) == slow_digit10_count(data));
        }
    }
}

TEST_CASE("encode", "[base10]") {
    SECTION("encode_lut") {
        char buffer[2], std_buffer[4];
        for (unsigned i{}; i < 100u; i++) {
            std::memcpy(buffer, &kon::detail::base10_encode_lut[i * 2u], 2);
            std::to_chars(std_buffer, std_buffer + 4, 1000 + i);
            REQUIRE(std::memcmp(buffer, std_buffer + 2, 2) == 0);
        }
    }
    SECTION("uint_encode") {
        constexpr std::size_t uint32_max_digits10 =
            kon::base10_count(std::numeric_limits<std::uint32_t>::max());
        char buffer[uint32_max_digits10];
        char std_buffer[uint32_max_digits10];
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<std::uint32_t> dist(0);
        for (unsigned i{}; i < 200u; i++) {
            auto number = dist(gen);
            std::size_t len = kon::base10_count(number);
            kon::base10_uint_encode(buffer + len, number);
            auto result = std::to_chars(std_buffer, std_buffer + sizeof(std_buffer), number);
            REQUIRE(result.ec == std::errc());
            std::size_t std_len = result.ptr - std_buffer;
            REQUIRE(std_len == len);
            REQUIRE(std::memcmp(buffer, std_buffer, len) == 0);
        }
    }
}