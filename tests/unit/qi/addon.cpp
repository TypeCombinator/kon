#include <kon/qi/struct.hpp>

namespace qi_addon_test {
struct s_foo {
    int a;
    double b;
    const char c;
    char d;

    template <typename addon_host_type = s_foo>
    struct addon_register {
        KON_QI_ADDON_INIT();

        KON_QI_ADDON_M(a, 0, nullptr);
        KON_QI_ADDON_M(b, 'X');
        KON_QI_ADDON_M(c);
    };
};

static_assert(kon::qi::has_internal_addon<s_foo>);

using s_foo_minfo = kon::qi::s_reflect<s_foo>;
using s_foo_addon = s_foo_minfo::addon_type;

static constexpr auto s_foo_addon_a = s_foo_addon::get_m(s_foo_minfo::addon_tag<0>());
static_assert(s_foo_addon_a.size() == 2);
static_assert(s_foo_addon_a.get<0>() == 0);
static_assert(s_foo_addon_a.get<1>() == nullptr);

static constexpr auto s_foo_addon_b = s_foo_addon::get_m(s_foo_minfo::addon_tag<1>());
static_assert(s_foo_addon_b.size() == 1);
static_assert(s_foo_addon_b.get<0>() == 'X');

static constexpr auto s_foo_addon_c = s_foo_addon::get_m(s_foo_minfo::addon_tag<2>());
static_assert(s_foo_addon_c.size() == 0);

static constexpr auto s_foo_addon_d = s_foo_addon::get_m(s_foo_minfo::addon_tag<3>());
static_assert(s_foo_addon_c.size() == 0);

struct s_bar {
    int a;
    double b;
    const char c;
    char d;
};

} // namespace qi_addon_test

namespace kon::qi {
template <>
struct addon_register<qi_addon_test::s_bar> {
    using addon_host_type = qi_addon_test::s_bar;
    KON_QI_ADDON_INIT();

    KON_QI_ADDON_M(a, 0, nullptr);
    KON_QI_ADDON_M(b, 'X');
    KON_QI_ADDON_M(c);
};
} // namespace kon::qi

namespace qi_addon_test {
static_assert(!kon::qi::has_external_addon<s_foo>);
static_assert(kon::qi::has_external_addon<s_bar>);

using s_bar_minfo = kon::qi::s_reflect<s_bar>;
using s_bar_addon = s_bar_minfo::addon_type;

static constexpr auto s_bar_addon_a = s_bar_addon::get_m(s_bar_minfo::addon_tag<0>());
static_assert(s_bar_addon_a.size() == 2);
static_assert(s_bar_addon_a.get<0>() == 0);
static_assert(s_bar_addon_a.get<1>() == nullptr);

static constexpr auto s_bar_addon_b = s_bar_addon::get_m(s_bar_minfo::addon_tag<1>());
static_assert(s_bar_addon_b.size() == 1);
static_assert(s_bar_addon_b.get<0>() == 'X');

static constexpr auto s_bar_addon_c = s_bar_addon::get_m(s_bar_minfo::addon_tag<2>());
static_assert(s_bar_addon_c.size() == 0);

static constexpr auto s_bar_addon_d = s_bar_addon::get_m(s_bar_minfo::addon_tag<3>());
static_assert(s_bar_addon_c.size() == 0);
} // namespace qi_addon_test
