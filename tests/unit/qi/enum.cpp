#include <kon/qi/enum.hpp>

namespace qi_enum_test {
enum class e_foo {
    v2 = 1,
    v_4 = -5,
    v3,
};
using e_foo_infos = kon::qi::e_reflect<e_foo>;
static_assert(!e_foo_infos::is_continous());
static_assert(e_foo_infos::size() == 3);
static_assert(e_foo_infos::to_name(e_foo::v2) == std::string_view{"v2"});
static_assert(e_foo_infos::to_name(e_foo::v_4) == std::string_view{"v_4"});
static_assert(e_foo_infos::to_name(e_foo::v3) == std::string_view{"v3"});


enum class e_foo_c {
    v_1 = -1,
    v0,
    v1,
    v2,
};
using e_foo1_infos = kon::qi::e_reflect<e_foo_c>;
static_assert(e_foo1_infos::is_continous());
static_assert(e_foo1_infos::size() == 4);
static_assert(e_foo1_infos::to_name(e_foo_c::v_1) == std::string_view{"v_1"});
static_assert(e_foo1_infos::to_name(e_foo_c::v0) == std::string_view{"v0"});
static_assert(e_foo1_infos::to_name(e_foo_c::v1) == std::string_view{"v1"});
static_assert(e_foo1_infos::to_name(e_foo_c::v2) == std::string_view{"v2"});
} // namespace qi_enum_test