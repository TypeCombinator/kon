#include <kon/qi/struct.hpp>

namespace qi_struct_test {
struct s_foo0 { };

struct s_foo2 {
    int a[2];
};

struct s_foo3 {
    int a;
    int b;
    int c;
};

static_assert(kon::qi::member_count<s_foo0>() == 0);
static_assert(kon::qi::member_count<s_foo2>() == 2);
static_assert(kon::qi::member_count<s_foo3>() == 3);
static_assert(kon::qi::member_count<s_foo3, 0, 4>() == 3);
// static constexpr std::size_t r =  kon::qi::member_count<s_foo3, 0, 3>();

using s_foo3_minfo = kon::qi::s_reflect<qi_struct_test::s_foo3>;
static_assert(s_foo3_minfo::member_name<0>() == std::string_view{"a"});
static_assert(s_foo3_minfo::member_name<1>() == std::string_view{"b"});
static_assert(s_foo3_minfo::member_name<2>() == std::string_view{"c"});
} // namespace qi_struct_test