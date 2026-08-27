#include <kon/qi/enum.hpp>

namespace qi_enum_test {
enum class e_foo {
    v0 = 1,
    v1 = -5,
    v2,
};
using e_foo_infos = kon::qi::e_reflect<e_foo, -10, 10>;
static_assert(!e_foo_infos::is_continuous());
static_assert(e_foo_infos::size() == 3);
static_assert(e_foo_infos::to_name(e_foo::v2) == std::string_view{"v2"});
static_assert(e_foo_infos::to_name(e_foo::v1) == std::string_view{"v1"});
static_assert(e_foo_infos::to_name(e_foo::v2) == std::string_view{"v2"});
static_assert(e_foo_infos::to_vindex("v8") == std::string_view::npos);
static_assert(e_foo_infos::to_vindex("v0") != std::string_view::npos);
static_assert(e_foo_infos::to_vindex("v1") != std::string_view::npos);
static_assert(e_foo_infos::to_vindex("v2") != std::string_view::npos);
static_assert(e_foo_infos::to_value_from_vindex(e_foo_infos::to_vindex("v0")) == e_foo::v0);
static_assert(e_foo_infos::to_value_from_vindex(e_foo_infos::to_vindex("v1")) == e_foo::v1);
static_assert(e_foo_infos::to_value_from_vindex(e_foo_infos::to_vindex("v2")) == e_foo::v2);


enum class e_foo_c {
    v0 = -1,
    v1,
    v2,
    v3,
};
using e_foo_c_infos = kon::qi::e_reflect<e_foo_c, -10, 10>;
static_assert(e_foo_c_infos::is_continuous());
static_assert(e_foo_c_infos::size() == 4);
static_assert(e_foo_c_infos::to_vindex("v8") == std::string_view::npos);
static_assert(e_foo_c_infos::to_vindex("v0") != std::string_view::npos);
static_assert(e_foo_c_infos::to_vindex("v1") != std::string_view::npos);
static_assert(e_foo_c_infos::to_vindex("v3") != std::string_view::npos);
static_assert(e_foo_c_infos::to_name(e_foo_c::v0) == std::string_view{"v0"});
static_assert(e_foo_c_infos::to_name(e_foo_c::v1) == std::string_view{"v1"});
static_assert(e_foo_c_infos::to_name(e_foo_c::v2) == std::string_view{"v2"});
static_assert(e_foo_c_infos::to_name(e_foo_c::v3) == std::string_view{"v3"});
static_assert(e_foo_c_infos::to_value_from_vindex(e_foo_c_infos::to_vindex("v0")) == e_foo_c::v0);
static_assert(e_foo_c_infos::to_value_from_vindex(e_foo_c_infos::to_vindex("v1")) == e_foo_c::v1);
static_assert(e_foo_c_infos::to_value_from_vindex(e_foo_c_infos::to_vindex("v2")) == e_foo_c::v2);
static_assert(e_foo_c_infos::to_value_from_vindex(e_foo_c_infos::to_vindex("v3")) == e_foo_c::v3);
} // namespace qi_enum_test