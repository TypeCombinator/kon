#include <kon/chrono/timebase.hpp>
#include <catch2/catch_template_test_macros.hpp>

TEST_CASE("basic", "[timebase]") {
    struct timespec ts;
    int ret = ::clock_gettime(CLOCK_REALTIME, &ts);
    REQUIRE(ret == 0);

    struct tm local_tm;
    auto ret_tm = ::localtime_r(&ts.tv_sec, &local_tm);
    REQUIRE(ret_tm != nullptr);

    ret = kon::timebase::init();
    REQUIRE(ret == 0);
    REQUIRE(kon::timebase::m_gmt_offset == local_tm.tm_gmtoff);
}