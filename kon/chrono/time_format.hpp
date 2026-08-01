// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef TIME_FORMAT_E62EDD10_9C8D_452C_8357_9372B9D3EF37
#define TIME_FORMAT_E62EDD10_9C8D_452C_8357_9372B9D3EF37
#include <kon/chrono/fast_date.hpp>
#include <kon/base10.hpp>
#include <string_view>

namespace kon {
// It's usaully defined as a thread_local variable.
struct ymd_hms_format_context {
    uint8_t date_end;
    char buffer[31];
    std::int64_t seconds{1ll << 63};
    std::int32_t days{1 << 31};
};

constexpr unsigned hms_length = sizeof("00:00:00") - 1u;

// seconds: [0, 86400)
// out: "hh:mm:ss"
static inline void seconds_to_hms_string(char out[hms_length], std::uint32_t seconds) noexcept {
    std::uint8_t hours = (static_cast<std::uint64_t>(seconds) * 2443359173u) >> 43;
    seconds -= static_cast<uint32_t>(hours) * static_cast<uint32_t>(3600);

    std::uint8_t minutes = (seconds * 34953u) >> 21;
    seconds -= static_cast<uint16_t>(minutes) * static_cast<uint16_t>(60);

    std::memcpy(out, &kon::detail::base10_encode_lut[hours * 2], 2);
    out += 2;
    *out++ = ':';
    std::memcpy(out, &kon::detail::base10_encode_lut[minutes * 2], 2);
    out += 2;
    *out++ = ':';
    std::memcpy(out, &kon::detail::base10_encode_lut[seconds * 2], 2);
}

static inline std::string_view
    seconds_to_ymd_hms_string(ymd_hms_format_context& ctx, std::int64_t seconds) noexcept {
    if (seconds == ctx.seconds) [[likely]] {
        return std::string_view{ctx.buffer, ctx.date_end + hms_length};
    }
    std::int32_t days = static_cast<std::int32_t>(seconds / 86400);
    seconds = seconds % 86400;
    if (seconds < 0) [[unlikely]] {
        seconds += 86400;
        days--;
    }
    char* buffer = ctx.buffer;
    if (days != ctx.days) [[unlikely]] {
        kon::date ymd = kon::fast_date::to_date(days);
        std::int32_t year = ymd.year;
        if (year < 0) [[unlikely]] {
            year = static_cast<uint32_t>(-year);
            *buffer++ = '-';
        }
        buffer += kon::base10_count(static_cast<std::uint32_t>(year));
        kon::base10_uint_encode(buffer, year);
        *buffer++ = '-';
        std::memcpy(buffer, &kon::detail::base10_encode_lut[ymd.month * 2], 2);
        buffer += 2;
        *buffer++ = '-';
        std::memcpy(buffer, &kon::detail::base10_encode_lut[ymd.day * 2], 2);
        buffer += 2;
        *buffer++ = ' ';

        ctx.date_end = static_cast<uint8_t>(buffer - ctx.buffer);
        ctx.days = days;
    }
    seconds_to_hms_string(ctx.buffer + ctx.date_end, static_cast<std::uint32_t>(seconds));
    return std::string_view{ctx.buffer, ctx.date_end + hms_length};
}

} // namespace kon
#endif // time_format.hpp