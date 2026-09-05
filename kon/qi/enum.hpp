// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef ENUM_6D7EAEC9_15FB_46FD_8465_783CB02DD621
#define ENUM_6D7EAEC9_15FB_46FD_8465_783CB02DD621
#include <kon/qi/name.hpp>
#include <kon/qi/addon.hpp>

namespace kon {
//
namespace qi {
template <std::size_t N, typename ET>
struct enum_information {
    std::size_t m_size;
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

constexpr int default_enum_value_range[1][2] = {
    {-128, 127}
};

template <auto& ValueRange, std::size_t VRN>
consteval std::size_t enum_total_space() noexcept {
    std::size_t space{};
    for (std::size_t i{}; i < VRN; i++) {
        if (ValueRange[i][1] < ValueRange[i][0]) {
            return 0;
        }
        space += (ValueRange[i][1] - ValueRange[i][0]) + 1;
    }
    return space;
}

template <typename ET, auto MinEv, auto MaxEv>
struct enum_information_maker {
    static constexpr std::size_t space = MaxEv - MinEv + 1;

    template <auto... Ns>
    static consteval void
        make_impl(auto& enum_infos, std::string_view& prefix, kon::index_sequence<Ns...>) {
        constexpr std::string_view pnames[] = {pretty_value_name<static_cast<ET>(MinEv + Ns)>()...};

        std::size_t size = enum_infos.m_size;
        for (std::size_t i{}; i < space; i++) {
            std::string_view name = pnames[i];
            if (!name.starts_with('(')) {
                if (size == 0) {
                    prefix = pretty_name_prefix(name);
                }
                name.remove_prefix(prefix.size());
                enum_infos.m_values[size] = static_cast<ET>(MinEv + i);
                enum_infos.m_names[size] = name;
                size++;
            }
        }
        enum_infos.m_size = size;
    }

    static consteval void make(auto& enum_infos, std::string_view& prefix) {
        make_impl(enum_infos, prefix, kon::make_index_sequence<space>{});
    }
};

template <typename ET, std::size_t TotalSpace, auto& ValueRanges, std::size_t VRN>
consteval enum_information<TotalSpace, ET> make_enum_information_impl() noexcept {
    enum_information<TotalSpace, ET> enum_info{};

    [&enum_info]<auto... Is>(kon::index_sequence<Is...>) {
        std::string_view prefix{};
        (enum_information_maker<ET, ValueRanges[Is][0], ValueRanges[Is][1]>::make(enum_info, prefix),
         ...);
        std::size_t size = enum_info.m_size;
        if (size > 0) {
            enum_info.m_min = enum_info.m_values[0];
            enum_info.m_max = enum_info.m_values[size - 1];
        }
        enum_info.m_pretty_prefix = prefix;
    }(kon::make_index_sequence<VRN>{});
    return enum_info;
}

template <typename ET, auto& ValueRanges>
consteval auto make_enum_infomation() noexcept {
    using vr_type = std::remove_reference_t<decltype(ValueRanges)>;
    static_assert(std::rank_v<vr_type> == 2);
    static_assert(std::extent_v<vr_type, 1> == 2);
    constexpr std::size_t vrn = std::extent_v<vr_type, 0>;
    static_assert(vrn > 0);

    constexpr std::size_t space = enum_total_space<ValueRanges, vrn>();
    static_assert(space > 0, "The value_range must be in ascending order and non-overlapping.");

    constexpr enum_information<space, ET> enum_infos =
        make_enum_information_impl<ET, space, ValueRanges, vrn>();
    return enum_infos.template shrink<enum_infos.m_size>();
}

template <typename ET>
struct e_reflect {
    static consteval auto addon() noexcept {
        if constexpr (requires() { typename addon_register<ET>::addon_host_type; }) {
            return addon_register<ET>{};
        } else {
            return addon_register<void>{};
        }
    }

    using addon_type = decltype(addon());

    using underlying_type = std::underlying_type_t<ET>;
    using type = ET;

    static consteval auto enum_information_select() noexcept {
        if constexpr (requires() { addon_type::value_range; }) {
            return make_enum_infomation<ET, addon_type::value_range>();
        } else {
            return make_enum_infomation<ET, default_enum_value_range>();
        }
    }

    static constexpr auto sm_info = enum_information_select();

    static consteval bool is_continuous() noexcept {
        return sm_info.m_is_continuous;
    }

    static consteval std::size_t size() noexcept {
        return sm_info.m_size;
    }

    static consteval ET min() noexcept {
        return sm_info.m_min;
    }

    static consteval ET max() noexcept {
        return sm_info.m_max;
    }

    static consteval std::string_view pretty_name_prefix() noexcept {
        return sm_info.m_pretty_prefix;
    }

    static constexpr std::string_view to_name(ET value, std::string_view invalid = {}) noexcept {
        underlying_type uv = static_cast<underlying_type>(value);
        if constexpr (is_continuous()) {
            constexpr underlying_type min_uv = static_cast<underlying_type>(min());
            if ((min_uv <= uv) && (uv <= static_cast<underlying_type>(max()))) [[likely]] {
                return sm_info.m_names[uv - min_uv];
            }
        } else {
            std::size_t start = 0;
            std::size_t end = size();
            while (start < end) {
                std::size_t m = (start + end) >> 1;
                underlying_type muv = static_cast<underlying_type>(sm_info.m_values[m]);
                if (muv == uv) {
                    return sm_info.m_names[m];
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
        const auto& names = sm_info.m_names;
        constexpr std::size_t size = sm_info.m_size;
        for (std::size_t i{}; i < size; i++) {
            if (names[i] == name) {
                return i;
            }
        }
        return std::string_view::npos;
    }

    static constexpr ET to_value_from_rank(std::size_t rank) noexcept {
        return sm_info.m_values[rank];
    }

    template <ET e>
    static consteval auto addon_tag() noexcept {
        return kon::qi::addon_tag<e>{};
    }
};
} // namespace qi
} // namespace kon
#endif // enum.hpp