#include <kon/qi/struct.hpp>
#include <kon/qi/addon.hpp>

namespace qi_addon_test {

struct s_foo {
    int a;
    double b;
    char c;
    char d;

    KON_QI_ADDON_INSIDE(s_foo) {
        using type = s_foo;
        KON_QI_ADDON_M(a, 0, 1);
        KON_QI_ADDON_M(b, 'X');
        KON_QI_ADDON_M(c);
    };
};

template <typename ADDON, std::size_t I>
consteval auto addon_get_m() noexcept {
    constexpr auto m_addr = kon::qi::s_reflect<typename ADDON::type>::sm_pp_info.template get<I>();
    if constexpr (requires() { ADDON::get_m(kon::qi::addon_tag<m_addr>{}); }) {
        return ADDON::get_m(kon::qi::addon_tag<m_addr>{});
    } else {
        return kon::qi::value_pack<>();
    }
}

static_assert(kon::qi::has_internal_addon<s_foo>);

using s_foo_minfo = kon::qi::s_reflect<s_foo>;
using s_foo_addon = kon::qi::addon_get<s_foo>;

static constexpr auto s_foo_addon_a = addon_get_m<s_foo_addon, 0>();
static_assert(s_foo_addon_a.get<0>() == 0);
static_assert(s_foo_addon_a.get<1>() == 1);

static constexpr auto s_foo_addon_b = addon_get_m<s_foo_addon, 1>();
static_assert(s_foo_addon_b.get<0>() == 'X');

static constexpr auto s_foo_addon_c = addon_get_m<s_foo_addon, 2>();
static_assert(s_foo_addon_c.size() == 0);

static constexpr auto s_foo_addon_d = addon_get_m<s_foo_addon, 3>();
static_assert(s_foo_addon_c.size() == 0);

} // namespace qi_addon_test
