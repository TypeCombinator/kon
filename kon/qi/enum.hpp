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
    ET m_values[N];
    ET m_min, m_max;
    std::string_view m_names[N];
    std::string_view m_pretty_prefix;
    std::size_t m_size{};
    bool m_is_continious;
};

template <typename ET, int MinEv, int MaxEv>
consteval enum_information<MaxEv - MinEv + 1, ET> make_enum_infomation() noexcept {
    constexpr std::size_t N = MaxEv - MinEv + 1;
    enum_information<N, ET> enum_infos{};
    [&enum_infos]<auto... Ns>(kon::index_sequence<Ns...>) {
        constexpr std::string_view pnames[] = {
            qi::pretty_value_name<static_cast<ET>(MinEv + Ns)>()...};
        bool is_continious{true};
        std::size_t prev = N;
        for (std::size_t i{}; i < N; i++) {
            std::string_view name = pnames[i];
            if (!name.starts_with('(')) {
                if ((prev != N) && (prev + 1 != i)) {
                    is_continious = false;
                }
                prev = i;
                std::size_t size = enum_infos.m_size;
                enum_infos.m_values[size] = static_cast<ET>(MinEv + i);
                enum_infos.m_names[size] = name; // FIXME: Shouldn't store pretty name!
                enum_infos.m_size = size + 1;
            }
        }
        if (enum_infos.m_size > 0) {
            enum_infos.m_pretty_prefix = qi::pretty_name_prefix(enum_infos.m_names[0]);
            enum_infos.m_min = enum_infos.m_values[0];
            enum_infos.m_max = enum_infos.m_values[enum_infos.m_size - 1];
        }
        enum_infos.m_is_continious = is_continious;
    }(kon::make_index_sequence<N>());
    return enum_infos;
}

template <typename ET>
struct e_reflect {
    using underlying_type = std::underlying_type_t<ET>;
    using type = ET;

    static constexpr auto sm_infos = make_enum_infomation<ET, -128, 127>();

    static consteval bool is_continous() noexcept {
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

    static constexpr std::string_view
        to_pretty_name(ET ev, std::string_view invalid = {}) noexcept {
        underlying_type uev = static_cast<underlying_type>(ev);
        if constexpr (is_continous()) {
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

    static consteval std::size_t pretty_name_prefix_size() noexcept {
        return sm_infos.m_pretty_prefix.size();
    }

    static constexpr std::string_view to_name(ET ev, std::string_view invalid = {}) noexcept {
        std::string_view r = to_pretty_name(ev);
        r.remove_prefix(pretty_name_prefix_size());
        return r;
    }
};
} // namespace qi
} // namespace kon
#endif // enum.hpp