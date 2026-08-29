// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef STRUCT_432BBF93_B816_4A5C_963E_0EDA8E33212F
#define STRUCT_432BBF93_B816_4A5C_963E_0EDA8E33212F
#include <kon/qi/utility.hpp>
#include <kon/qi/name.hpp>

#define KON_FAST_FWD1(_arg_) static_cast<decltype(_arg_)&&>(_arg_)

namespace kon {
//
namespace qi {
template <std::size_t N>
struct cure_all {
    template <class T>
    operator T() const noexcept;
};

template <class T, std::size_t Start = 0, std::size_t End = 256>
consteval std::size_t member_count() noexcept {
    static_assert(Start < End, "Too many members!");

    constexpr std::size_t Middle = (Start + End) >> 1;
    constexpr int r = []<std::size_t... Ns>(kon::index_sequence<Ns...>) {
        if constexpr (not requires { T{cure_all<Ns>{}...}; }) {
            return 1;
        } else if constexpr (
            requires { T{cure_all<Ns>{}...}; }
            and not requires { T{cure_all<Ns>{}..., cure_all<Middle + 1>{}}; }) {
            return 0;
        } else {
            return -1;
        }
    }(kon::make_index_sequence<Middle>());

    if constexpr (r > 0) {
        return member_count<T, Start, Middle>();
    } else if constexpr (r < 0) {
        return member_count<T, Middle + 1, End>();
    } else {
        return Middle;
    }
}

namespace detail {
template <typename T>
extern const T __eobj{};

struct __mtype {
    char _m_;
};

struct member_guider {
    static constexpr std::string_view name{"_m_"};
    static constexpr std::string_view fname{fun_str_nttp<&detail::__eobj<detail::__mtype>._m_>()};
    static constexpr std::size_t offset = fname.rfind(name);
    static constexpr std::string_view prefix{fname.data() + offset - 2, 2};
    static constexpr std::size_t rgarbage = fname.size() - (offset + name.size());
};

template <auto M>
consteval std::string_view member_name() noexcept {
    std::string_view name = fun_str_nttp<M>();
    std::size_t offset = name.rfind(member_guider::prefix);
    name.remove_prefix(offset + member_guider::prefix.size());
    name.remove_suffix(member_guider::rgarbage);
    return name;
}

template <auto& obj>
constexpr void mvisit_as_nttp(size_constant<3>, auto&& fun) noexcept {
    auto& [m0, m1, m2] = obj;
    fun.template operator()<&m0, &m1, &m2>();
}
} // namespace detail

template <std::size_t N>
struct struct_information {
    std::string_view m_names[N + 1];
};

template <std::size_t N, typename T>
consteval struct_information<N> make_struct_information() noexcept {
    struct_information<N> info;
    detail::mvisit_as_nttp<detail::__eobj<T>>(kon::size_constant<N>{}, [&info]<auto... Ms>() {
        std::string_view names[N] = {detail::member_name<Ms>()...};
        for (std::size_t i{}; i < N; i++) {
            info.m_names[i] = names[i];
        }
    });
    return info;
}

template <typename T>
struct s_reflect {
    static constexpr std::size_t sm_size = member_count<T>();
    static constexpr auto sm_info = make_struct_information<sm_size, T>();

    template <std::size_t I>
    static consteval std::string_view member_name() noexcept {
        return sm_info.m_names[I];
    }
};

} // namespace qi
} // namespace kon

#endif // struct.hpp