// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef ADDON_1A34AF33_3863_4910_984C_743C4E4BD1CD
#define ADDON_1A34AF33_3863_4910_984C_743C4E4BD1CD
#include <kon/qi/pack.hpp>

#define KON_QI_ADDON_INSIDE(_type_)                                                                \
    template <typename ADDON_T = _type_>                                                           \
    struct addon_register

#define KON_QI_ADDON_M(_m_, ...)                                                                   \
    static consteval auto get_m(kon::qi::addon_tag<&(CODECL<ADDON_T>._m_)>) noexcept {             \
        return kon::qi::value_pack<__VA_ARGS__>{};                                                 \
    }

#define KON_QI_ADDON_INIT()                                                                        \
    template <auto maddr>                                                                          \
    static consteval auto get_m(kon::qi::addon_tag<maddr>) noexcept {                              \
        return kon::qi::value_pack<>{};                                                            \
    }

namespace kon {
//
namespace qi {
template <auto V>
struct addon_tag { };

template <typename T>
concept has_internal_addon = requires() { typename T::template addon_register<T>; };

template <typename T>
using addon_get = T::template addon_register<T>;

struct addon_register_empty { };
} // namespace qi
} // namespace kon


#endif // addon.hpp