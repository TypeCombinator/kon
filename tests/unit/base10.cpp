#include <kon/base10.hpp>
#include <random>
#include <catch2/catch_test_macros.hpp>

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
    static_assert(kon::base10_count(std::numeric_limits<std::uint32_t>::max()) == 10);
    static_assert(kon::base10_count(0ul) == 1);
    static_assert(kon::base10_count(std::numeric_limits<std::uint64_t>::max()) == 20);

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