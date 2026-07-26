#include <random>
#include <catch2/catch_template_test_macros.hpp>
#include <kon/chrono/fast_date.hpp>

static kon::date std_to_date(std::int32_t day_number) noexcept {
    std::chrono::days d{day_number};
    std::chrono::sys_days tp{d};
    std::chrono::year_month_day ymd{tp};

    return kon::date{
        static_cast<std::int32_t>(ymd.year()),
        static_cast<std::uint8_t>(static_cast<unsigned>(ymd.month())),
        static_cast<std::uint8_t>(static_cast<unsigned>(ymd.day())),
    };
}

static std::int32_t
    std_days_between(int y1, unsigned m1, unsigned d1, int y2, unsigned m2, unsigned d2) {
    std::chrono::year_month_day date1{
        std::chrono::year{y1}, std::chrono::month{m1}, std::chrono::day{d1}};
    std::chrono::year_month_day date2{
        std::chrono::year{y2}, std::chrono::month{m2}, std::chrono::day{d2}};

    std::chrono::sys_days tp1{date1};
    std::chrono::sys_days tp2{date2};

    std::chrono::days diff = tp2 - tp1;
    return diff.count();
}

TEST_CASE("basic", "[fast_date]") {
    REQUIRE(std_days_between(0, 2, 29, 1970, 1, 1) == 719469);
    REQUIRE(kon::fast_date::to_rata_die(0, 2, 29) == -719469);


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::int32_t> dist(-30000 * 365, 30000 * 365);
    for (std::uint32_t i{}; i < 100; i++) {
        std::int32_t day_number = dist(gen);
        auto std_date = std_to_date(day_number);
        auto tpx_date = kon::fast_date::to_date(day_number);

        REQUIRE(std_date.year == tpx_date.year);
        REQUIRE(std_date.month == tpx_date.month);
        REQUIRE(std_date.day == tpx_date.day);
    }
}