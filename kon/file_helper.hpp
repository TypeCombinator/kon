// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef FILE_HELPER_F096EED7_C6AD_44B1_A05D_5EC47CA6B5E7
#define FILE_HELPER_F096EED7_C6AD_44B1_A05D_5EC47CA6B5E7

#include <string>
#include <memory>

namespace kon::file_helper {

std::unique_ptr<std::uint8_t[]> read_all(std::string file_name, size_t &file_size);

} // namespace kon::file_helper

#endif /* file_helper.hpp */