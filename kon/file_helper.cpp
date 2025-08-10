// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#include "file_helper.hpp"
#include <fstream>
#include <fcntl.h>

namespace kon::file_helper {

std::unique_ptr<std::uint8_t[]> read_all(const std::string &file_name, size_t &file_size) {
    std::ifstream file(file_name, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        return nullptr;
    }
    std::ptrdiff_t file_pos = file.tellg();

    if (file_pos < 0) {
        return nullptr;
    }

    auto buffer = std::make_unique_for_overwrite<std::uint8_t[]>(file_pos + 1);
    file.seekg(0, std::ios::beg);
    if (!file.read(reinterpret_cast<char *>(buffer.get()), file_pos)) {
        return nullptr;
    }
    file_size = file_pos;
    // Make it compatible with C-Style Strings.
    buffer[file_pos] = '\0';
    return buffer;
}

int rename_swap(
    const std::string &file0_name,
    const std::string &file1_name,
    const std::string &temp_file_name) noexcept {
    // 0: {file0(content0), file1(content1)}

    // 1: {file0(content0), temp_file(content1)}
    if (std::rename(file1_name.c_str(), temp_file_name.c_str()) != 0) {
        return -1;
    }
    // 2: {file1(content0), temp_file(content1)}
    if (std::rename(file0_name.c_str(), file1_name.c_str()) != 0) {
        std::rename(temp_file_name.c_str(), file1_name.c_str());
        return -2;
    }
    // 3: {file1(content0), file0(content1)}
    if (std::rename(temp_file_name.c_str(), file0_name.c_str()) != 0) {
        rename(file1_name.c_str(), file0_name.c_str());
        rename(temp_file_name.c_str(), file1_name.c_str());
        return -3;
    }
    return 0;
}

#ifdef __linux__
int swap(const std::string &file0_name, const std::string &file1_name) noexcept {
    return renameat2(AT_FDCWD, file0_name.c_str(), AT_FDCWD, file1_name.c_str(), RENAME_EXCHANGE);
}
#endif

}; // namespace kon::file_helper