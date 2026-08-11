// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef LOG_DC1032E1_AB1A_48E1_B92E_01031700561E
#define LOG_DC1032E1_AB1A_48E1_B92E_01031700561E
#include <kon/log/log_frontend.hpp>
#include <kon/log.hpp>
#include <kon/chrono/timebase.hpp>
#include <time.h>

#define KON_AS_STR(x)    #x
#define KON_STRINGIFY(x) KON_AS_STR(x)


#define KON_LOG_PRINT(_logger_, _level_str_, _fmt_, ...)                                           \
    do {                                                                                           \
        struct timespec __ts;                                                                      \
        ::clock_gettime(CLOCK_REALTIME, &__ts);                                                    \
        std::string_view __ymd_hms = kon::seconds_to_ymd_hms_string(                               \
            kon::logger::ymd_hms_context, __ts.tv_sec + kon::timebase::m_gmt_offset);              \
        (_logger_).print(                                                                          \
            "{}.{} " _level_str_ "@{} {}(" KON_STRINGIFY(__LINE__) "): " _fmt_ "\n",               \
            __ymd_hms,                                                                             \
            __ts.tv_nsec,                                                                          \
            (_logger_).tls_prefix,                                                                 \
            __FILE_NAME__,                                                                         \
            ##__VA_ARGS__);                                                                        \
    } while (0)

#define KON_LOG_PRINT_FLEVEL(_logger_, _level_str_, _level_, _fmt_, ...)                           \
    do {                                                                                           \
        if ((_logger_).is_level_enabled(_level_)) {                                                \
            KON_LOG_PRINT(_logger_, _level_str_, _fmt_, ##__VA_ARGS__);                            \
        }                                                                                          \
    } while (0)

#endif /* log.hpp */