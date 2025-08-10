#include <catch2/catch_test_macros.hpp>
#include <kon/inerting.hpp>

namespace kon_interting_test {
struct count {
    count(std::uint32_t &ctor_count, std::uint32_t &dtor_count) noexcept
        : m_ctor_count{ctor_count}
        , m_dtor_count(dtor_count) {
        m_ctor_count++;
    }

    ~count() noexcept {
        m_dtor_count++;
    }

    std::uint32_t &m_ctor_count;
    std::uint32_t &m_dtor_count;
};
} // namespace kon_interting_test

TEST_CASE("basic", "[inerting]") {
    SECTION("default construction") {
        kon::inerting<int> i;
        REQUIRE_NOTHROW(i.construct(42));
        REQUIRE(i.get() == 42);
        REQUIRE_NOTHROW(i.destruct());
    }

    SECTION("object construction") {
        kon::inerting<std::string> s;

        REQUIRE_NOTHROW(s.construct("test"));
        REQUIRE(s.get() == "test");

        SECTION("modification") {
            s.get() = "modified";
            REQUIRE(s.get() == "modified");
        }

        REQUIRE_NOTHROW(s.destruct());
    }

    SECTION("const access") {
        kon::inerting<int> i;
        i.construct(100);

        const auto &ci = i;
        REQUIRE(ci.get() == 100);
    }

    SECTION("exception safety") {
        struct throwing_type {
            throwing_type() {
                throw 42;
            }
        };

        kon::inerting<throwing_type> t;
        REQUIRE_THROWS(t.construct());
    }

    SECTION("destruction safety") {
        struct non_destructible {
            ~non_destructible() noexcept(false) {
                throw 42;
            }
        };

        kon::inerting<non_destructible> nd;
        nd.construct();
        REQUIRE_THROWS(nd.destruct());
    }

    SECTION("count") {
        uint32_t ctor_count = 0, dtor_count = 0;
        {
            kon::inerting<kon_interting_test::count> obj;
            obj.construct(ctor_count, dtor_count);
            obj.destruct();
        }
        REQUIRE(ctor_count == 1);
        REQUIRE(dtor_count == 1);
    }
}