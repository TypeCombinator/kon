#include <kon/chrono/time_spec.hpp>
#include <catch2/catch_template_test_macros.hpp>

static_assert(std::is_trivial_v<kon::time_spec_ns>);

TEST_CASE("basic", "[time_spec]") {
    SECTION("add") {
        auto result = kon::time_spec_ns{10, 300'000'000ll} + kon::time_spec_ns{5, 800'000'000ll};
        REQUIRE(result.seconds == 16);
        REQUIRE(result.subseconds == 100'000'000ll);

        result = kon::time_spec_ns{-1, -300'000'000ll} + kon::time_spec_ns{-2, -800'000'000ll};
        REQUIRE(result.seconds == -5);
        REQUIRE(result.subseconds == 900'000'000ll);

        result = kon::time_spec_ns{10, 300'000'000ll} + kon::time_spec_ns{5, -800'000'000ll};
        REQUIRE(result.seconds == 14);
        REQUIRE(result.subseconds == 500'000'000ll);

        result = kon::time_spec_ns{-3, -300'000'000ll} + kon::time_spec_ns{1, 800'000'000ll};
        REQUIRE(result.seconds == -2);
        REQUIRE(result.subseconds == 500'000'000ll);
    }
    SECTION("sub") {
        auto result = kon::time_spec_ns{10, 300'000'000ll} - kon::time_spec_ns{5, 800'000'000ll};
        REQUIRE(result.seconds == 4);
        REQUIRE(result.subseconds == 500'000'000ll);

        result = kon::time_spec_ns{-1, -300'000'000ll} - kon::time_spec_ns{-2, -800'000'000ll};
        REQUIRE(result.seconds == 1);
        REQUIRE(result.subseconds == 500'000'000ll);

        result = kon::time_spec_ns{5, 300'000'000ll} - kon::time_spec_ns{5, 800'000'000ll};
        REQUIRE(result.seconds == -1);
        REQUIRE(result.subseconds == 500'000'000ll);

        result = kon::time_spec_ns{1, 500'000'000ll} - kon::time_spec_ns{0, 800'000'000ll};
        REQUIRE(result.seconds == 0);
        REQUIRE(result.subseconds == 700'000'000ll);
    }

    SECTION("compare") {
        REQUIRE(kon::time_spec_ns{0, 100} < kon::time_spec_ns{0, 1000});
        REQUIRE(kon::time_spec_ns{0, 1000} > kon::time_spec_ns{0, 100});
        REQUIRE(kon::time_spec_ns{-1, 1000} < kon::time_spec_ns{0, 1000});
        REQUIRE(kon::time_spec_ns{0, 1000} > kon::time_spec_ns{-1, 1000});
        REQUIRE(kon::time_spec_ns{100, 1000} == kon::time_spec_ns{100, 1000});
        REQUIRE(kon::time_spec_ns{100, 1000} != kon::time_spec_ns{10, 1000});
    }
}