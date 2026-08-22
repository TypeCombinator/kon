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
    bool m_is_continious;
    ET m_values[N];
    ET m_min, m_max;
    std::string_view m_names[N];
    std::string_view m_pretty_prefix;

    template <std::size_t SN>
    consteval enum_information<SN, ET> shrink() const noexcept {
        enum_information<SN, ET> r{};
        std::size_t s = m_size;
        r.m_size = s;
        r.m_is_continious = m_is_continious;
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

template <typename ET, int MinEv, int MaxEv>
consteval auto make_enum_infomation() noexcept {
    constexpr std::size_t N = MaxEv - MinEv + 1;
    using enum_information_t = enum_information<N, ET>;
    constexpr enum_information_t enum_infos = []() noexcept {
        enum_information_t enum_infos{};
        [&enum_infos]<auto... Ns>(kon::index_sequence<Ns...>) {
            constexpr std::string_view pnames[] = {
                qi::pretty_value_name<static_cast<ET>(MinEv + Ns)>()...};
            bool is_continious{true};
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
                        is_continious = false;
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
            enum_infos.m_is_continious = is_continious;
        }(kon::make_index_sequence<N>());
        return enum_infos;
    }();
    return enum_infos.template shrink<enum_infos.m_size>();
}

template <typename ET, int MinEv = -128, int MaxEv = 127>
struct e_reflect {
    using underlying_type = std::underlying_type_t<ET>;
    using type = ET;

    static constexpr auto sm_infos = make_enum_infomation<ET, MinEv, MaxEv>();

    static consteval bool is_continuous() noexcept {
        return sm_infos.m_is_continious;
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

    static constexpr std::string_view to_name(ET ev, std::string_view invalid = {}) noexcept {
        underlying_type uev = static_cast<underlying_type>(ev);
        if constexpr (is_continuous()) {
            constexpr underlying_type m = static_cast<underlying_type>(min());
            if ((m <= uev) && (uev <= static_cast<underlying_type>(max()))) [[likely]] {
                return sm_infos.m_names[uev - m];
            }
        } else {
            std::size_t start = 0;
            std::size_t end = size();
            while (start < end) {
                std::size_t m = (start + end) >> 1;
                underlying_type mv = static_cast<underlying_type>(sm_infos.m_values[m]);
                if (mv == uev) {
                    return sm_infos.m_names[m];
                }
                if (static_cast<underlying_type>(mv) < uev) {
                    start = m + 1;
                } else {
                    end = m;
                }
            }
        }
        return invalid;
    }

    static constexpr std::size_t to_vindex(std::string_view en) noexcept {
        const auto &names = sm_infos.m_names;
        constexpr std::size_t size = sm_infos.m_size;
        for (std::size_t i{}; i < size; i++) {
            if (names[i] == en) {
                return i;
            }
        }
        return std::string_view::npos;
    }

    static constexpr ET to_value_from_vindex(std::size_t index) noexcept {
        return sm_infos.m_values[index];
    }
};
} // namespace qi
} // namespace kon
#endif // enum.hpp