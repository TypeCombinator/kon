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
} // namespace qi_member_test