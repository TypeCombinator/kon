#include <kon/bitset.hpp>
#include <random>
#include <catch2/catch_template_test_macros.hpp>

TEMPLATE_TEST_CASE("bit_for_each_t", "[bitset]", std::uint32_t, std::uint64_t) {
    constexpr std::uint32_t bit_number = sizeof(TestType) * 8;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<TestType> dist(0);

    uint8_t expect_out[64], out[64];
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