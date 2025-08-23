#include <catch2/catch_test_macros.hpp>
#include <kon/scope.hpp>

TEST_CASE("scope_exit", "[scope]") {
    int count = 0;
    {
        auto guard = kon::scope_exit{[&count]() noexcept {
            count++;
        }};
    }
    REQUIRE(count == 1);

    {
        kon::scope_exit guard{[&count]() noexcept {
            count++;
        }};
    }
    REQUIRE(count == 2);
}

namespace scope_test {
static int global_count = 0;

static inline void fd_close_global_count(int fd) noexcept {
    if (fd >= 0) {
        global_count++;
    }
}

static inline void fd_close_count(int fd, int &count) noexcept {
    if (fd >= 0) {
        count++;
    }
}
} // namespace scope_test

TEST_CASE("lw_scope", "[scope]") {
    SECTION("global") {
        int fd = 1;
        {
            kon::lw_scope<scope_test::fd_close_global_count>::guard sc{fd};
            // Minimal memory footprint!
            STATIC_REQUIRE(sizeof(sc) == sizeof(fd));
        }
        REQUIRE(scope_test::global_count == 1);

        {
            kon::lw_scope<scope_test::fd_close_global_count>::guard sc{fd};
        }
        REQUIRE(scope_test::global_count == 2);

        {
            kon::lw_scope<[](int fd) noexcept {
                if (fd >= 0) {
                    scope_test::global_count++;
                }
            }>::guard sc{fd};
        }
        REQUIRE(scope_test::global_count == 3);

        {
            kon::lw_scope<scope_test::fd_close_global_count>::guard sc{fd};
            sc._0 = -1;
        }
        REQUIRE(scope_test::global_count == 3);

        {
            kon::lw_scope<scope_test::fd_close_global_count>::guard sc{-1};
        }
        REQUIRE(scope_test::global_count == 3);
    }

    SECTION("local") {
        int fd = 1;
        int count = 0;
        {
            kon::lw_scope<scope_test::fd_close_count>::guard sc{fd, std::ref(count)};
        }
        REQUIRE(count == 1);

        {
            kon::lw_scope<scope_test::fd_close_count>::guard sc{fd, std::ref(count)};
        }
        REQUIRE(count == 2);

        {
            kon::lw_scope<[](int fd, int &count) noexcept -> void {
                if (fd >= 0) {
                    count++;
                }
            }>::guard sc{fd, std::ref(count)};
        }
        REQUIRE(count == 3);

        {
            kon::lw_scope<scope_test::fd_close_count>::guard sc{fd, std::ref(count)};
            sc._0 = -1;
        }
        REQUIRE(count == 3);

        {
            kon::lw_scope<scope_test::fd_close_count>::guard sc{-1, std::ref(count)};
        }
        REQUIRE(count == 3);
    }
}