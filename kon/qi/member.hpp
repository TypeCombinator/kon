// SPDX-FileCopyrightText: 2026 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef MEMBER_692AFE14_9E46_46F0_B589_38EC5223AFD9
#define MEMBER_692AFE14_9E46_46F0_B589_38EC5223AFD9
#include <memory>

namespace kon {
//
namespace qi {
template <typename T, typename M>
constexpr std::size_t offset_of(M T::*field) noexcept {
    union U {
        unsigned char buffer[sizeof(T)];
        T t;

        constexpr U() noexcept {
        }

        constexpr ~U() noexcept {
        }
    } u{};

    std::size_t start = 0;
    std::size_t end = sizeof(T);
    const void *target = static_cast<const void *>(std::addressof(u.t.*field));
    while (start < end) {
        std::size_t m = (start + end) >> 1;
        if (u.buffer + m == target) {
            return m;
        }
        if (u.buffer + m < target) {
            start = m + 1;
        } else {
            end = m;
        }
    }
    return 0;
}
} // namespace qi
} // namespace kon
#endif // member.hpp