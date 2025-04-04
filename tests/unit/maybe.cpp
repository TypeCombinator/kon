#include <kon/maybe.hpp>
#include <catch2/catch_test_macros.hpp>

static_assert(sizeof(kon::maybe<float>) == 4);
static_assert(sizeof(kon::maybe<double>) == 8);
static_assert(sizeof(kon::maybe<int *>) == 8);
static_assert(sizeof(kon::maybe<int>) == 8);

TEST_CASE("float", "[float]") {
    kon::maybe<float> a;
    CHECK(!a);

    kon::maybe<float> b{std::in_place, 0.1};
    CHECK(b);

    kon::maybe<float> c{kon::nullopt};
    CHECK(!c);

    kon::maybe<float> d = 0.1;
    CHECK(d);

    kon::maybe<float> e{d};
    CHECK(e);

    kon::maybe<float> f = kon::nullopt;
    CHECK(!f);

    bool flag = false;
    d.and_then([&flag](float a) -> void { //
        flag = true;
    });
    CHECK(flag);

    d.reset();
    CHECK(!d);

    flag = false;
    d.and_then([&flag](float a) -> void { //
        flag = true;
    });
    CHECK(!flag);
}