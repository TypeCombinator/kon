// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef LOG_SINK_CONSOLE_E4B34C01_B711_4481_AF87_467036251108
#define LOG_SINK_CONSOLE_E4B34C01_B711_4481_AF87_467036251108
#include <kon/log/log_frontend.hpp>

namespace kon {

struct log_sink_console {
    FILE* m_file{stdout};
    static int write_all(void* self, std::string_view data);
    static int flush_all(void* self);
    static int sync_all(void* self);
    static int clear_all(void* self);

    static const logger::sink_interface sink_if;
};

} // namespace kon
#endif // log_sink_console.hpp