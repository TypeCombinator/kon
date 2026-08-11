// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#include <kon/log/log_sink_console.hpp>

namespace kon {

int log_sink_console::write_all(void *v_self, std::string_view data) {
    auto self = static_cast<log_sink_console *>(v_self);

    auto written = std::fwrite(data.data(), 1, data.size(), self->m_file);
    return 0;
}

int log_sink_console::flush_all(void *v_self) {
    auto self = static_cast<log_sink_console *>(v_self);
    return std::fflush(self->m_file);
}

int log_sink_console::sync_all(void *v_self) {
    return 0;
}

int log_sink_console::clear_all(void *v_self) {
    // Do nothing
    return 0;
}

const logger::sink_interface log_sink_console::sink_if{
    write_all,
    flush_all,
    sync_all,
    clear_all,
};

} // namespace kon