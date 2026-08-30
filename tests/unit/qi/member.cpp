#include <kon/qi/member.hpp>

namespace qi_member_test {
struct s_foo3 {
    int a;
    char b;
    const double c;
};

static_assert(kon::qi::member_name<&s_foo3::a>() == std::string_view{"a"});
static_assert(kon::qi::member_name<&s_foo3::b>() == std::string_view{"b"});
static_assert(kon::qi::member_name<&s_foo3::c>() == std::string_view{"c"});


static_assert(kon::qi::is_member_pointer_v<&s_foo3::a>);
static_assert(kon::qi::is_member_pointer_v<&s_foo3::c>);
static_assert(!kon::qi::is_member_pointer_v<s_foo3{}>);
static_assert(!kon::qi::is_member_pointer_v<int{}>);
static_assert(!kon::qi::is_member_pointer_v<nullptr>);
} // namespace qi_member_test
