// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef TIMEBASE_D3BC8235_A02F_471C_8413_4B34FB3291CD
#define TIMEBASE_D3BC8235_A02F_471C_8413_4B34FB3291CD
#include <cstdint>

namespace kon {

struct timebase {
    // Greenwich Mean Time offset.
    static inline std::int32_t m_gmt_offset{};
    static int init();
};
} // namespace kon
#endif // timebase.hpp