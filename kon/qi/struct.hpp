// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef STRUCT_432BBF93_B816_4A5C_963E_0EDA8E33212F
#define STRUCT_432BBF93_B816_4A5C_963E_0EDA8E33212F
#include <kon/qi/utility.hpp>
#include <kon/qi/name.hpp>

#define KON_FAST_FWD1(_arg_) static_cast<decltype(_arg_)&&>(_arg_)

// A const object declaration.
template <typename T>
extern const T CODECL{};

// A struct declaration.
struct SDECL {
    char Z;
};

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

consteval std::size_t member_offset(
    std::size_t start,
    std::size_t end,
    const unsigned char* init,
    const void* target) noexcept {
    while (start < end) {
        std::size_t mid = (start + end) >> 1;
        const unsigned char* addr = init + mid;
        if (addr == target) {
            return mid;
        } else if (addr < target) {
            start = mid + 1;
        } else {
            end = mid;
        }
    }
    return 0;
}

template <auto& obj>
constexpr void mvisit_as_nttp(size_constant<0>, auto&& fun) noexcept {
    fun.template operator()<>();
}

template <auto& obj>
constexpr void mvisit_as_nttp(size_constant<3>, auto&& fun) noexcept {
    auto& [m0, m1, m2] = obj;
    fun.template operator()<&m0, &m1, &m2>();
}
} // namespace detail

template <std::size_t N>
struct struct_information {
    std::string_view m_names[N];
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
        std::size_t offset[N] = {detail::member_offset(0, sizeof(T), init, Ms)...};
        for (std::size_t i{}; i < N; i++) {
            info.m_offsets[i] = offset[i];
        }
    });
    return info;
}

template <typename T>
struct s_reflect {
    static constexpr std::size_t sm_size = member_count<T>();
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
};

} // namespace qi
} // namespace kon

#endif // struct.hpp