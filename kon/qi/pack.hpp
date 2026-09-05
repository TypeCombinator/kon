// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef PACK_18471EA5_EC9B_4807_9CE6_3F6C9FD9DD4F
#define PACK_18471EA5_EC9B_4807_9CE6_3F6C9FD9DD4F
#include <kon/qi/utility.hpp>

namespace kon {
//
namespace qi {
template <auto... Vs>
struct value_pack {
    static consteval std::size_t size() noexcept {
        return sizeof...(Vs);
    }

    template <std::size_t I>
    static consteval auto get() noexcept {
        return value_pack_element<I>(Vs...);
    }

    static consteval auto visit(auto &&fun) noexcept {
        static_cast<decltype(fun) &&>(fun).template operator()<Vs...>();
    }
};
} // namespace qi
} // namespace kon
#endif // pack.hpp