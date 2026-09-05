// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef STRUCT_432BBF93_B816_4A5C_963E_0EDA8E33212F
#define STRUCT_432BBF93_B816_4A5C_963E_0EDA8E33212F
#include <kon/qi/addon.hpp>
#include <kon/qi/name.hpp>
#include <kon/qi/detail/mcall_each.hpp>

// A const object declaration.
template <typename T>
extern const T CODECL{};

// A struct declaration.
struct SDECL {
    char Z;
};

namespace kon {
template <typename T, typename U>
constexpr auto&& forward_binding(auto&& u) noexcept {
    if constexpr (std::is_lvalue_reference_v<T>) {
        return u;
    } else {
        return static_cast<U&&>(u);
    }
}

namespace qi {
template <std::size_t N>
struct cure_all {
    template <class T>
    operator T() const noexcept;
};

template <class T, std::size_t Start = 0, std::size_t End = 128>
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
struct member_guider {
    static constexpr std::string_view fname{fun_str_nttp<&CODECL<SDECL>.Z>()};
    static constexpr std::size_t offset = fname.rfind('Z');
    static constexpr std::string_view prefix{fname.data() + offset - 2, 2};
    static constexpr std::size_t rgarbage = fname.size() - (offset + 1);
};

template <auto M>
consteval std::string_view member_name() noexcept {
    std::string_view name = fun_str_nttp<M>();
    std::size_t offset = name.rfind(member_guider::prefix);
    offset += member_guider::prefix.size();
    return {name.data() + offset, name.size() - (offset + member_guider::rgarbage)};
}

template <auto& obj>
constexpr auto mvisit_as_nttp(size_constant<0>, auto&& fun) noexcept {
    return fun.template operator()<>();
}

template <typename T>
constexpr decltype(auto) mvisit(size_constant<0>, T&& obj, auto&& fun) noexcept {
    return fun();
}

#define FWD_BINDING_T(_m_) forward_binding<T, decltype(_m_)>(_m_)

#define KON_CALL_VA(_i_, ...)                                                                      \
    template <auto& obj>                                                                           \
    constexpr auto mvisit_as_nttp(kon::size_constant<_i_>, auto&& fun) noexcept {                  \
        auto&& [__VA_ARGS__] = obj;                                                                \
        return fun.template operator()<KON_CALL_EACH##_i_(&, __VA_ARGS__)>();                      \
    }                                                                                              \
    template <typename T>                                                                          \
    constexpr decltype(auto) mvisit(kon::size_constant<_i_>, T&& obj, auto&& fun) noexcept {       \
        auto&& [__VA_ARGS__] = KON_FAST_FWD1(obj);                                                 \
        return fun(KON_CALL_EACH##_i_(FWD_BINDING_T, __VA_ARGS__));                                \
    }

#include "kon/qi/detail/mcall_va.inc"
#undef KON_CALL_VA
#undef FWD_BINDING_T
} // namespace detail

template <std::size_t N>
struct struct_information {
    std::string_view m_names[N];
    // TODO: Defer computation until use.
    std::size_t m_offsets[N];
};

template <std::size_t N, typename T>
consteval struct_information<N> make_struct_information() noexcept {
    struct_information<N> info;
    detail::mvisit_as_nttp<CODECL<T>>(kon::size_constant<N>{}, [&info]<auto... Ms>() {
        std::string_view names[N] = {detail::member_name<Ms>()...};
        for (std::size_t i{}; i < N; i++) {
            info.m_names[i] = names[i];
        }
    });

    union U {
        unsigned char buffer[sizeof(T)];
        T t;

        constexpr U() noexcept {
        }

        constexpr ~U() noexcept {
        }
    };

    detail::mvisit_as_nttp<CODECL<U>.t>(kon::size_constant<N>{}, [&info]<auto... Ms>() {
        const unsigned char* init = CODECL<U>.buffer;
        const void* targets[N] = {Ms...};
        std::size_t msizes[N] = {sizeof(*Ms)...};
        std::size_t offset = 0;
        for (std::size_t i{}; i < N; i++) {
            const void* target = targets[i];
            while ((init + offset) < target) {
                offset++;
            }
            info.m_offsets[i] = offset;
            offset += (msizes[i] & (~std::size_t{1}));
        }
    });
    return info;
}

template <typename T>
struct s_reflect {
    static constexpr std::size_t sm_size = member_count<T>();
    // Preprocessed information.
    static constexpr auto sm_pp_info =
        detail::mvisit_as_nttp<CODECL<T>>(size_constant<sm_size>{}, []<auto... Vs>() {
            return kon::qi::value_pack<Vs...>{};
        });
    static constexpr auto sm_info = make_struct_information<sm_size, T>();

    static consteval std::size_t size() noexcept {
        return sm_size;
    }

    template <std::size_t I>
    static consteval std::string_view member_name() noexcept {
        return sm_info.m_names[I];
    }

    static constexpr std::string_view member_name(std::size_t I) noexcept {
        return sm_info.m_names[I];
    }

    template <std::size_t I>
    static consteval std::size_t member_offset() noexcept {
        return sm_info.m_offsets[I];
    }

    static constexpr std::size_t member_offset(std::size_t I) noexcept {
        return sm_info.m_offsets[I];
    }

    template <std::size_t I>
    static constexpr auto&& member_get(auto&& obj) noexcept {
        return detail::mvisit(
            size_constant<sm_size>{}, KON_FAST_FWD1(obj), kon::value_pack_element<I>);
    }

    template <std::size_t I>
    using member_type = std::remove_cvref_t<decltype(*sm_pp_info.template get<I>())>;

    static void foreach(auto&& fun) {
        [&]<auto... Is>(kon::index_sequence<Is...>) {
            (KON_FAST_FWD1(fun).template operator()<Is>(), ...);
        }(kon::make_index_sequence<sm_size>{});
    }
};

} // namespace qi
} // namespace kon

#endif // struct.hpp