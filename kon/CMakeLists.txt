# SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
#
# SPDX-License-Identifier: BSD 3-Clause

add_library(kon STATIC
    hash/md5.cpp
    base16.cpp
    conv.cpp
    dev_mem.cpp
    file_helper.cpp
    shm.cpp
)
target_include_directories(kon PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..)