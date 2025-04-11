// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef TYPES_F8436028_1D08_400B_90D5_B15ACC7C8432
#define TYPES_F8436028_1D08_400B_90D5_B15ACC7C8432

#include <cstddef>

#define KON_CARRAY_SIZE(_carray_) (sizeof(_carray_) / sizeof(_carray_[0]))

namespace kon {

template <typename T, std::size_t N>
constexpr size_t array_size(const T (&)[N]) noexcept {
    return N;
}

} // namespace kon

#endif /* types.hpp */