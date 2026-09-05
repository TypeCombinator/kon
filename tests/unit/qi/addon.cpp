#include <kon/qi/struct.hpp>
#include <kon/qi/addon.hpp>

namespace qi_addon_test {

struct s_foo {
    int a;
    double b;
    const char c;
    char d;

    KON_QI_ADDON_INSIDE(s_foo) {
        KON_QI_ADDON_INIT();

        KON_QI_ADDON_M(a, 0, nullptr);
        KON_QI_ADDON_M(b, 'X');
        KON_QI_ADDON_M(c);
    };
};

static_assert(kon::qi::has_internal_addon<s_foo>);

static constexpr auto s_foo_maddrs = kon::qi::s_reflect<s_foo>::sm_maddrs;
using s_foo_addon = kon::qi::addon_get<s_foo>;

static constexpr auto s_foo_addon_a =
    s_foo_addon::get_m(kon::qi::addon_tag<s_foo_maddrs.get<0>()>{});
static_assert(s_foo_addon_a.size() == 2);
static_assert(s_foo_addon_a.get<0>() == 0);
static_assert(s_foo_addon_a.get<1>() == nullptr);

static constexpr auto s_foo_addon_b =
    s_foo_addon::get_m(kon::qi::addon_tag<s_foo_maddrs.get<1>()>{});
static_assert(s_foo_addon_b.size() == 1);
static_assert(s_foo_addon_b.get<0>() == 'X');

static constexpr auto s_foo_addon_c =
    s_foo_addon::get_m(kon::qi::addon_tag<s_foo_maddrs.get<2>()>{});
static_assert(s_foo_addon_c.size() == 0);

static constexpr auto s_foo_addon_d =
    s_foo_addon::get_m(kon::qi::addon_tag<s_foo_maddrs.get<3>()>{});
static_assert(s_foo_addon_c.size() == 0);

} // namespace qi_addon_test
