#include <random>
#include <catch2/catch_template_test_macros.hpp>
#include <kon/chrono/time_format.hpp>
#include <format>

static std::string std_seconds_to_date_time_string(int64_t seconds) {
    std::chrono::sys_seconds tp{std::chrono::seconds{seconds}};
    auto days = floor<std::chrono::days>(tp);
    std::chrono::year_month_day ymd = std::chrono::year_month_day{days};
    std::chrono::hh_mm_ss<std::chrono::seconds> hms =
        std::chrono::hh_mm_ss<std::chrono::seconds>{tp - days};

    return std::format(
        "{}-{:02}-{:02} {:02}:{:02}:{:02}",
        static_cast<int>(ymd.year()),
        static_cast<unsigned>(ymd.month()),
        static_cast<unsigned>(ymd.day()),
        hms.hours().count(),
        hms.minutes().count(),
        hms.seconds().count());
}

TEST_CASE("basic", "[time_format]") {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::int64_t> dist(-30000ll * 365 * 86400, 30000ll * 365 * 86400);
    kon::ymd_hms_format_context ctx{};
    REQUIRE(ctx.seconds == std::numeric_limits<std::int64_t>::min());
    REQUIRE(ctx.days == std::numeric_limits<std::int32_t>::min());
    for (std::uint32_t i{}; i < 100; i++) {
        std::int64_t seconds = dist(gen);
        std::string std_result = std_seconds_to_date_time_string(seconds);
        std::string_view result = kon::seconds_to_ymd_hms_string(ctx, seconds);
        REQUIRE(std_result == result);
    }
}