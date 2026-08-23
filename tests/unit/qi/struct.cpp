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
} // namespace qi_struct_test