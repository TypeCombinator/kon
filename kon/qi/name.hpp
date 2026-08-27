// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef NAME_50B1C3B5_5521_4D01_AB74_4F0AB0278A4E
#define NAME_50B1C3B5_5521_4D01_AB74_4F0AB0278A4E
#include <string_view>
#if defined(__GNUC__) || defined(__clang__)
    #define CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
    #define CURRENT_FUNCTION __FUNCSIG__
#else
    #include <source_location>
    #define CURRENT_FUNCTION std::source_location::current().function_name()
#endif
namespace kon {
//
namespace qi {
template <typename T>
consteval const char *fun_str() noexcept {
    return CURRENT_FUNCTION;
}

template <auto T>
consteval const char *fun_str_nttp() noexcept {
    return CURRENT_FUNCTION;
}

struct type_guider {
    static constexpr std::string_view name{"int"};
    static constexpr std::string_view fname{fun_str<int>()};
    static constexpr std::size_t offset = fname.find(name);
    static constexpr std::size_t garbage = fname.size() - name.size();
};

struct value_guider {
    static constexpr std::string_view name{"nullptr"};
    static constexpr std::string_view fname{fun_str_nttp<nullptr>()};
    static constexpr std::size_t offset = fname.find(name);
    static constexpr std::size_t garbage = fname.size() - name.size();
};

template <typename T>
consteval std::string_view pretty_type_name() noexcept {
    constexpr std::string_view n{fun_str<T>()};
#if defined(_MSC_VER) && not defined(__clang__)
    constexpr std::string_view n1{n.data() + type_guider::offset, n.size() - type_guider::garbage};
    constexpr std::string_view n2 = n1.substr(0, 7);
    constexpr std::size_t sp = n2.find(' ');
    if ((sp != std::string_view::npos) && (n1[sp + 1] != '*')) {
        return std::string_view{
            n1.data() + (sp + 1),
            n1.size() - (sp + 1),
        };
    }
    return n1;
#else
    return {n.data() + type_guider::offset, n.size() - type_guider::garbage};
#endif
}

template <auto T>
consteval std::string_view pretty_value_name() noexcept {
    constexpr std::string_view n{fun_str_nttp<T>()};
    return {n.data() + value_guider::offset, n.size() - value_guider::garbage};
}

consteval std::string_view pretty_name_prefix(std::string_view v) noexcept {
    std::size_t offset = v.rfind("::");
    if (offset != std::string_view::npos) {
        return std::string_view{v.data(), offset + 2};
    }
    return v;
}
} // namespace qi
} // namespace kon
#endif // name.hpp