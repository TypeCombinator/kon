// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef ENUM_6D7EAEC9_15FB_46FD_8465_783CB02DD621
#define ENUM_6D7EAEC9_15FB_46FD_8465_783CB02DD621
#include <kon/qi/utility.hpp>
#include <kon/qi/name.hpp>

namespace kon {
//
namespace qi {
template <std::size_t N, typename ET>
struct enum_information {
    std::size_t m_size{};
    bool m_is_continuous;
    ET m_values[N];
    ET m_min, m_max;
    std::string_view m_names[N];
    std::string_view m_pretty_prefix;

    template <std::size_t SN>
    consteval enum_information<SN, ET> shrink() const noexcept {
        enum_information<SN, ET> r;
        std::size_t s = m_size;
        r.m_size = s;
        r.m_is_continuous = m_is_continuous;
        for (std::size_t i{}; i < s; i++) {
            r.m_values[i] = m_values[i];
        }
        for (std::size_t i{}; i < s; i++) {
            r.m_names[i] = m_names[i];
        }
        r.m_min = m_min;
        r.m_max = m_max;
        r.m_pretty_prefix = m_pretty_prefix;
        return r;
    }
};

template <typename ET, std::size_t N, int MinEv, int MaxEv>
consteval enum_information<N, ET> make_enum_information_impl() noexcept {
    enum_information<N, ET> enum_infos{};
    [&enum_infos]<auto... Ns>(kon::index_sequence<Ns...>) {
        constexpr std::string_view pnames[] = {
            qi::pretty_value_name<static_cast<ET>(MinEv + Ns)>()...};
        bool is_continuous{true};
        std::size_t prev = N;
        std::string_view prefix{};
        std::size_t size{};
        for (std::size_t i{}; i < N; i++) {
            std::string_view name = pnames[i];
            if (!name.starts_with('(')) {
                if (size == 0) {
                    prefix = pretty_name_prefix(name);
                    enum_infos.m_pretty_prefix = prefix;
                }
                name.remove_prefix(prefix.size());
                if ((prev != N) && (prev + 1 != i)) {
                    is_continuous = false;
                }
                prev = i;
                enum_infos.m_values[size] = static_cast<ET>(MinEv + i);
                enum_infos.m_names[size] = name;
                size++;
            }
        }
        if (size > 0) {
            enum_infos.m_size = size;
            enum_infos.m_min = enum_infos.m_values[0];
            enum_infos.m_max = enum_infos.m_values[enum_infos.m_size - 1];
        }
        enum_infos.m_is_continuous = is_continuous;
    }(kon::make_index_sequence<N>());
    return enum_infos;
}

template <typename ET, int MinEv, int MaxEv>
consteval auto make_enum_infomation() noexcept {
    constexpr std::size_t N = MaxEv - MinEv + 1;
    constexpr enum_information<N, ET> enum_infos =
        make_enum_information_impl<ET, N, MinEv, MaxEv>();
    return enum_infos.template shrink<enum_infos.m_size>();
}

template <typename ET, int MinEv = -128, int MaxEv = 127>
struct e_reflect {
    using underlying_type = std::underlying_type_t<ET>;
    using type = ET;

    static constexpr auto sm_infos = make_enum_infomation<ET, MinEv, MaxEv>();

    static consteval bool is_continuous() noexcept {
        return sm_infos.m_is_continuous;
    }

    static consteval std::size_t size() noexcept {
        return sm_infos.m_size;
    }

    static consteval ET min() noexcept {
        return sm_infos.m_min;
    }

    static consteval ET max() noexcept {
        return sm_infos.m_max;
    }

    static consteval std::string_view pretty_name_prefix() noexcept {
        return sm_infos.m_pretty_prefix;
    }

    static constexpr std::string_view to_name(ET value, std::string_view invalid = {}) noexcept {
        underlying_type uv = static_cast<underlying_type>(value);
        if constexpr (is_continuous()) {
            constexpr underlying_type min_uv = static_cast<underlying_type>(min());
            if ((min_uv <= uv) && (uv <= static_cast<underlying_type>(max()))) [[likely]] {
                return sm_infos.m_names[uv - min_uv];
            }
        } else {
            std::size_t start = 0;
            std::size_t end = size();
            while (start < end) {
                std::size_t m = (start + end) >> 1;
                underlying_type muv = static_cast<underlying_type>(sm_infos.m_values[m]);
                if (muv == uv) {
                    return sm_infos.m_names[m];
                }
                if (muv < uv) {
                    start = m + 1;
                } else {
                    end = m;
                }
            }
        }
        return invalid;
    }

    static constexpr std::size_t to_rank(std::string_view name) noexcept {
        const auto &names = sm_infos.m_names;
        constexpr std::size_t size = sm_infos.m_size;
        for (std::size_t i{}; i < size; i++) {
            if (names[i] == name) {
                return i;
            }
        }
        return std::string_view::npos;
    }

    static constexpr ET to_value_from_rank(std::size_t rank) noexcept {
        return sm_infos.m_values[rank];
    }
};
} // namespace qi
} // namespace kon
#endif // enum.hpp