// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef TIME_SPEC_F341A903_484C_426E_9030_740A96F3DC44
#define TIME_SPEC_F341A903_484C_426E_9030_740A96F3DC44
#include <cstdint>
#include <compare>

namespace kon {
template <std::uint64_t Ratio>
struct time_spec {
    std::int64_t seconds;
    std::int64_t subseconds;

    static constexpr std::int64_t ratio = Ratio;

    time_spec() = default;

    time_spec(std::int64_t s, std::int64_t ss) noexcept
        : seconds{s}
        , subseconds{ss} { };

    template <std::uint64_t U>
    explicit constexpr time_spec(time_spec<U> other) noexcept {
        seconds = other.seconds;
        if constexpr (time_spec<U>::ratio > ratio) {
            constexpr std::uint64_t den = time_spec<U>::ratio / ratio;
            subseconds = other.subseconds / den;
        } else {
            constexpr std::uint64_t num = ratio / time_spec<U>::ratio;
            subseconds = other.subseconds * num;
        }
    }

    friend constexpr time_spec operator+(const time_spec &lhs, const time_spec &rhs) noexcept {
        std::int64_t subseconds = lhs.subseconds + rhs.subseconds;
        std::int64_t cary = subseconds / static_cast<std::int64_t>(Ratio);
        subseconds = subseconds % static_cast<std::int64_t>(Ratio);
        std::int64_t seconds = lhs.seconds + rhs.seconds;
        seconds += cary;
        // Normalize
        if (subseconds < 0) {
            seconds--;
            subseconds += Ratio;
        }
        return time_spec{seconds, subseconds};
    }

    friend constexpr time_spec operator-(const time_spec &lhs, const time_spec &rhs) noexcept {
        std::int64_t subseconds = lhs.subseconds - rhs.subseconds;
        std::int64_t cary = subseconds / static_cast<std::int64_t>(Ratio);
        subseconds = subseconds % static_cast<std::int64_t>(Ratio);
        std::int64_t seconds = lhs.seconds - rhs.seconds;
        seconds -= cary;
        // Normalize
        if (subseconds < 0) {
            seconds--;
            subseconds += Ratio;
        }
        return time_spec{seconds, subseconds};
    }

    friend constexpr std::strong_ordering
        operator<=>(const time_spec &lhs, const time_spec &rhs) noexcept {
        if (auto cmp = lhs.seconds <=> rhs.seconds; cmp != 0) {
            return cmp;
        }
        return lhs.subseconds <=> rhs.subseconds;
    }

    friend bool operator==(const time_spec &lhs, const time_spec &rhs) noexcept = default;
};

using time_spec_ms = time_spec<1'000ull>;
using time_spec_us = time_spec<1'000'000ull>;
using time_spec_ns = time_spec<1'000'000'000ull>;

} // namespace kon

#endif // time_spec.hpp