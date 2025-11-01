// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef FILE_HELPER_F096EED7_C6AD_44B1_A05D_5EC47CA6B5E7
#define FILE_HELPER_F096EED7_C6AD_44B1_A05D_5EC47CA6B5E7

#include <memory>
#include <filesystem>

namespace kon::file_helper {

std::unique_ptr<std::uint8_t[]> read_all(const std::string &file_name, size_t &file_size);

// 0: {file0(content0), file1(content1)}
// 1: {file0(content0), temp_file(content1)}
// 2: {file1(content0), temp_file(content1)}
// 3: {file1(content0), file0(content1)}
int rename_swap(
    const std::string &file0_name,
    const std::string &file1_name,
    const std::string &temp_file_name) noexcept;

#ifdef __linux__
int swap(const std::string &file0_name, const std::string &file1_name) noexcept;
#endif

int create_file_directories(const std::filesystem::path &filename);

} // namespace kon::file_helper

#endif /* file_helper.hpp */