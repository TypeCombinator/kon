#include <kon/qi/struct.hpp>
#include <kon/qi/member.hpp>
#include <catch2/catch_test_macros.hpp>

namespace qi_struct_test {
struct s_foo0 { };

struct s_foo2 {
    int a[2];
};

struct s_foo3 {
    int a;
    char b;
    const double c;
};

static_assert(kon::qi::member_count<s_foo0>() == 0);
static_assert(kon::qi::member_count<s_foo2>() == 2);
static_assert(kon::qi::member_count<s_foo3>() == 3);
static_assert(kon::qi::member_count<s_foo3, 0, 4>() == 3);

// The parameter `End` must be greater than the number of elements
// static constexpr std::size_t r =  kon::qi::member_count<s_foo3, 0, 3>();


struct s_foo_cvref {
    int a;
    const int &a_lref;
    int &&a_rref;
    const int &&a_crref;
    int *c;
};

static_assert(kon::qi::member_count<s_foo_cvref>() == 5);

struct s_foo_lref {
    int &a;
};

// Notice: Non-const lvalue reference members are not supported!
// static_assert(kon::qi::member_count<s_foo_lref>() == 1);

using s_foo0_minfo = kon::qi::s_reflect<qi_struct_test::s_foo0>;
static_assert(s_foo0_minfo::size() == 0);

using s_foo3_minfo = kon::qi::s_reflect<qi_struct_test::s_foo3>;
static_assert(s_foo3_minfo::member_name<0>() == std::string_view{"a"});
static_assert(s_foo3_minfo::member_name<1>() == std::string_view{"b"});
static_assert(s_foo3_minfo::member_name<2>() == std::string_view{"c"});

static_assert(s_foo3_minfo::member_offset<0>() == kon::qi::offset_of(&s_foo3::a));
static_assert(s_foo3_minfo::member_offset<1>() == kon::qi::offset_of(&s_foo3::b));
static_assert(s_foo3_minfo::member_offset<2>() == kon::qi::offset_of(&s_foo3::c));

static_assert(std::is_same_v<s_foo3_minfo::member_type<0>, int>);
static_assert(std::is_same_v<s_foo3_minfo::member_type<1>, char>);
static_assert(std::is_same_v<s_foo3_minfo::member_type<2>, double>);
} // namespace qi_struct_test

using namespace qi_struct_test;

TEST_CASE("basic", "[struct]") {
    s_foo3 obj{0x123, 'x', 0.1};
    auto &a = s_foo3_minfo::member_get<0>(obj);
    auto &b = s_foo3_minfo::member_get<1>(obj);
    auto &c = s_foo3_minfo::member_get<2>(obj);
    REQUIRE(a == 0x123);
    REQUIRE(b == 'x');
    REQUIRE(c == 0.1);
    a = 0x321;
    b = 'y';
    REQUIRE(s_foo3_minfo::member_get<0>(obj) == 0x321);
    REQUIRE(s_foo3_minfo::member_get<1>(obj) == 'y');
    REQUIRE(s_foo3_minfo::member_get<2>(obj) == 0.1);

    s_foo3_minfo::foreach([&]<std::size_t I>() {
        if constexpr (I == 0) {
            REQUIRE(s_foo3_minfo::member_get<I>(obj) == 0x321);
        } else if constexpr (I == 1) {
            REQUIRE(s_foo3_minfo::member_get<I>(obj) == 'y');
        } else {
            REQUIRE(s_foo3_minfo::member_get<I>(obj) == 0.1);
        }
    });
}
}