# SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
#
# SPDX-License-Identifier: BSD 3-Clause

add_library(kon STATIC
    chrono/timebase.cpp
    hash/md5.cpp
    log/log_sink_circular_buffer.cpp
    log/log_sink_console.cpp
    log/log_sink_file.cpp
    log/log.cpp
    tools/bash.cpp
    base16.cpp
    conv.cpp
    dev_mem.cpp
    file_helper.cpp
    shm.cpp
)
target_include_directories(kon PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(kon PUBLIC
    fmt::fmt
)