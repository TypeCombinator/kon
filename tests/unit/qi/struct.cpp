#include <kon/qi/struct.hpp>
#include <kon/qi/member.hpp>

namespace qi_struct_test {
struct s_foo0 { };

struct s_foo2 {
    int a[2];
};

struct s_foo3 {
    int a;
    char b;
    int c;
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
} // namespace qi_struct_test