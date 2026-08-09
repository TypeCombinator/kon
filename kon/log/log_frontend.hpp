// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef LOG_FRONTEND_E7EDAB0E_F47C_4242_B71B_2EFD24A68056
#define LOG_FRONTEND_E7EDAB0E_F47C_4242_B71B_2EFD24A68056
#include <fmt/base.h>
#include <fmt/format.h>
#include <kon/chrono/time_format.hpp>

namespace kon {
enum class log_level : unsigned {
    none,
    critical,
    error,
    warning,
    information,
    debug,
    trace,
};

static constexpr auto log_level_underlying(log_level level) noexcept {
    return static_cast<unsigned>(level);
}

struct logger {
    struct sink_interface {
        int (*write_all)(void* self, std::string_view data);
        int (*flush_all)(void* self);
        int (*sync_all)(void* self); // Optional.
        int (*clear_all)(void* self);
    };

    void set_level(log_level level) noexcept {
        m_log_level = level;
    }

    log_level get_level() noexcept {
        return m_log_level;
    }

    bool is_level_enabled(log_level level) noexcept {
        return log_level_underlying(level) <= log_level_underlying(m_log_level);
    }

    int set_sink(const sink_interface& interface, void* sink) noexcept {
        m_sink_if = &interface;
        m_sink = sink;
        return 0;
    }

    template <typename... T>
    void print(fmt::format_string<T...> fmt, T&&... args) {
        auto buffer = fmt::memory_buffer();
        fmt::vargs<T...> va = {{args...}};
        fmt::detail::vformat_to(buffer, fmt, va);
        m_sink_if->write_all(m_sink, {buffer.data(), buffer.size()});
    }

    int flush_all() {
        return m_sink_if->flush_all(m_sink);
    }

    int sync_all() {
        return m_sink_if->sync_all(m_sink);
    }

    int clear_all() {
        return m_sink_if->clear_all(m_sink);
    }

    inline static thread_local std::string tls_prefix{};
    inline static thread_local kon::ymd_hms_format_context ymd_hms_context{};
   private:
    log_level m_log_level{log_level::none};

    const sink_interface* m_sink_if{nullptr};
    void* m_sink{nullptr};
};


} // namespace kon

#endif // log_frontend.hpp