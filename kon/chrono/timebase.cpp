// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#include <kon/chrono/timebase.hpp>
#include <time.h>

namespace kon {

int timebase::init() {
    struct timespec ts;
    int ret = ::clock_gettime(CLOCK_REALTIME, &ts);
    if (ret < 0) {
        return ret;
    }
    struct tm local_tm;
    if (::localtime_r(&ts.tv_sec, &local_tm) == nullptr) {
        return -1;
    }
    m_gmt_offset = local_tm.tm_gmtoff;
    return 0;
}

} // namespace kon