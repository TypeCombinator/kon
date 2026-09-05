#include <kon/qi/struct.hpp>
#include <kon/qi/enum.hpp>

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

template <typename T, char V = 'X'>
consteval bool test_struct_addon() noexcept {
    using minfo = kon::qi::s_reflect<T>;
    using addon = minfo::addon_type;

    constexpr auto m0_addon = addon::get_m(minfo::template addon_tag<0>());
    static_assert(m0_addon.size() == 2);
    static_assert(m0_addon.template get<0>() == 0);
    static_assert(m0_addon.template get<1>() == nullptr);

    constexpr auto m1_addon = addon::get_m(minfo::template addon_tag<1>());
    static_assert(m1_addon.size() == 1);
    static_assert(m1_addon.template get<0>() == V);

    constexpr auto m2_addon = addon::get_m(minfo::template addon_tag<2>());
    static_assert(m2_addon.size() == 0);

    constexpr auto m3_addon = addon::get_m(minfo::template addon_tag<3>());
    static_assert(m2_addon.size() == 0);
    return true;
}

static_assert(test_struct_addon<s_foo>());
static_assert(test_struct_addon<s_bar>());

template <typename T>
struct ts_x {
    int a;
    double b;
    const char c;
    char d;

    template <typename addon_host_type = ts_x>
    struct addon_register {
        KON_QI_ADDON_INIT();

        KON_QI_ADDON_M(a, 0, nullptr);
        KON_QI_ADDON_M(b, std::is_same_v<void, T> ? 'X' : 'Y');
        KON_QI_ADDON_M(c);
    };
};

static_assert(test_struct_addon<ts_x<void>>());
static_assert(test_struct_addon<ts_x<int>, 'Y'>());

enum class e_foo : int {
    a,
    b,
    c,
    d
};
} // namespace qi_addon_test

namespace kon::qi {
template <>
struct addon_register<qi_addon_test::e_foo> {
    using addon_host_type = qi_addon_test::e_foo;
    KON_QI_ADDON_INIT();
    KON_QI_ADDON_E(a, 0, nullptr);
    KON_QI_ADDON_E(b, 'X');
    KON_QI_ADDON_E(c);
};

} // namespace kon::qi

namespace qi_addon_test {
consteval bool test_enum_addon() noexcept {
    using minfo = kon::qi::e_reflect<e_foo>;
    using addon = minfo::addon_type;

    constexpr auto m0_addon = addon::get_m(minfo::addon_tag<e_foo::a>());
    static_assert(m0_addon.size() == 2);
    static_assert(m0_addon.template get<0>() == 0);
    static_assert(m0_addon.template get<1>() == nullptr);

    constexpr auto m1_addon = addon::get_m(minfo::addon_tag<e_foo::b>());
    static_assert(m1_addon.size() == 1);
    static_assert(m1_addon.template get<0>() == 'X');

    constexpr auto m2_addon = addon::get_m(minfo::addon_tag<e_foo::c>());
    static_assert(m2_addon.size() == 0);

    constexpr auto m3_addon = addon::get_m(minfo::addon_tag<e_foo::d>());
    static_assert(m2_addon.size() == 0);
    return true;
}

static_assert(test_enum_addon());

} // namespace qi_addon_test