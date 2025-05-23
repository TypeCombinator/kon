// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#include "file_helper.hpp"
#include <fstream>

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

}; // namespace kon::file_helper