// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef ADDON_1A34AF33_3863_4910_984C_743C4E4BD1CD
#define ADDON_1A34AF33_3863_4910_984C_743C4E4BD1CD
#include <kon/qi/pack.hpp>

// TODO: It may be better to declare a function without defining it.
#define KON_QI_ADDON_M(_m_, ...)                                                                   \
    static consteval auto of(kon::qi::addon_tag<&(CODECL<addon_host_type>._m_)>) noexcept {        \
        return kon::qi::value_pack<__VA_ARGS__>{};                                                 \
    }

#define KON_QI_ADDON_E(_e_, ...)                                                                   \
    static consteval auto of(kon::qi::addon_tag<addon_host_type::_e_>) noexcept {                  \
        return kon::qi::value_pack<__VA_ARGS__>{};                                                 \
    }

#define KON_QI_ADDON_INIT()                                                                        \
    template <auto maddr>                                                                          \
    static consteval auto of(kon::qi::addon_tag<maddr>) noexcept {                                 \
        return kon::qi::value_pack<>{};                                                            \
    }

namespace kon {
//
namespace qi {
template <auto V>
struct addon_tag { };

template <typename T>
struct addon_register {
    KON_QI_ADDON_INIT();
    // Empty addon.
};

template <typename T>
concept has_internal_addon = requires() { typename T::template addon_register<>; };

template <typename T>
concept has_external_addon = requires() { typename addon_register<T>::addon_host_type; };

} // namespace qi
} // namespace kon


#endif // addon.hpp