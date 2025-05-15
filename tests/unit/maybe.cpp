#include <kon/maybe.hpp>
#include <catch2/catch_test_macros.hpp>

static_assert(sizeof(kon::maybe<float>) == 4);
static_assert(sizeof(kon::maybe<double>) == 8);
static_assert(sizeof(kon::maybe<int*>) == 8);
static_assert(sizeof(kon::maybe<int>) == 8);

TEST_CASE("basic", "[maybe]") {
    SECTION("Default construction") {
        kon::maybe<int> m;
        REQUIRE_FALSE(m.has_value());
    }

    SECTION("Value construction") {
        kon::maybe<int> m(42);
        REQUIRE(m.has_value());
        REQUIRE(m.get() == 42);
    }

    SECTION("Nullopt construction") {
        kon::maybe<int> m(kon::nullopt);
        REQUIRE_FALSE(m.has_value());
    }

    SECTION("Copy construction") {
        kon::maybe<int> m1(42);
        kon::maybe<int> m2(m1);
        REQUIRE(m2.has_value());
        REQUIRE(m2.get() == 42);
    }

    SECTION("Move construction") {
        kon::maybe<int> m1(42);
        kon::maybe<int> m2(std::move(m1));
        REQUIRE(m2.has_value());
        REQUIRE(m2.get() == 42);
    }

    SECTION("Assignment") {
        kon::maybe<int> m;
        m = 42;
        REQUIRE(m.has_value());
        REQUIRE(m.get() == 42);

        m = kon::nullopt;
        REQUIRE_FALSE(m.has_value());
    }

    SECTION("Pointer specialization") {
        int number = 42;
        kon::maybe<int*> m(&number);
        REQUIRE(m.has_value());
        REQUIRE(*m.get() == 42);

        m = nullptr;
        REQUIRE_FALSE(m.has_value());
    }

    SECTION("Floating point specialization") {
        kon::maybe<double> m(3.14);
        REQUIRE(m.has_value());
        REQUIRE(m.get() == 3.14);

        m = std::numeric_limits<double>::quiet_NaN();
        REQUIRE_FALSE(m.has_value());
    }

    SECTION("Reset") {
        kon::maybe<int> m(42);
        m.reset();
        REQUIRE_FALSE(m.has_value());
    }

    SECTION("Emplace") {
        kon::maybe<std::pair<int, int>> m;
        m.emplace(1, 2);
        REQUIRE(m.has_value());
        REQUIRE(m.get().first == 1);
        REQUIRE(m.get().second == 2);
    }

    SECTION("And then") {
        kon::maybe<int> m(42);
        auto result = m.and_then([](int x) { return x * 2; });
        REQUIRE(result == 84);

        m = kon::nullopt;
        result = m.and_then([](int x) { return x * 2; });
        REQUIRE(result == 0);
    }
}

TEST_CASE("float", "[maybe]") {
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

TEST_CASE("custom", "[maybe]") {
    struct custom_nothing {
        static void make(int* location) {
            *location = -1;
        }

        static void reset(int& location) {
            location = -1;
        }

        static bool has(const int& v) {
            return v < 0;
        }
    };

    kon::maybe<int, custom_nothing> m;
    REQUIRE_FALSE(m.has_value());
    REQUIRE(m.get() == -1);

    m = 42;
    REQUIRE(m.has_value());
    REQUIRE(m.get() == 42);

    m = -2;
    REQUIRE_FALSE(m.has_value());
    REQUIRE(m.get() == -2);

    m = kon::nullopt;
    REQUIRE_FALSE(m.has_value());
    REQUIRE(m.get() == -1);

    m.reset();
    REQUIRE_FALSE(m.has_value());
    REQUIRE(m.get() == -1);
}