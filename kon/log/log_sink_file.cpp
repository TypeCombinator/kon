// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#include <kon/log/log_sink_file.hpp>
#include <kon/file_helper.hpp>

namespace kon {
int log_sink_file::initialize(
    const std::filesystem::path &filename,
    const std::string &mode) noexcept {
    m_filename = filename;
    if (kon::file_helper::create_file_directories(filename) != 0) {
        return -1;
    }
    auto file = std::fopen(filename.string().data(), mode.data());
    if (file == nullptr) {
        fmt::println("failed to crate the file: {}", filename.string());
        return -1;
    }
    auto current_pos = std::ftell(file);
    if (current_pos < 0) {
        std::fclose(file);
        return -1;
    }
    std::fseek(file, 0, SEEK_END);
    m_file_size = ftell(file);
    std::fseek(file, current_pos, SEEK_SET);
    m_file = file;
    return 0;
}

int log_sink_file::write_all(void *v_self, std::string_view data) {
    auto self = static_cast<log_sink_file *>(v_self);
    std::unique_lock<std::mutex> lock{self->m_lock};
    if (self->m_file == nullptr) {
        return -1;
    }
    auto data_size = data.size();
    if (self->m_file_size > self->m_file_size_limit) {
        return -1;
    }
    auto written = std::fwrite(data.data(), 1, data.size(), self->m_file);
    if (written < data_size) {
        // TODO: record
        return -1;
    }
    self->m_file_size += data.size();
    return 0;
}

int log_sink_file::flush_all(void *v_self) {
    auto self = static_cast<log_sink_file *>(v_self);
    std::unique_lock<std::mutex> lock{self->m_lock};
    if (self->m_file == nullptr) {
        return -1;
    }
    return std::fflush(self->m_file);
}

int log_sink_file::sync_all(void *v_self) {
    auto self = static_cast<log_sink_file *>(v_self);
    std::unique_lock<std::mutex> lock{self->m_lock};
    if (self->m_file == nullptr) {
        return -1;
    }
    return fsync((fileno(self->m_file)));
}

int log_sink_file::clear_all(void *v_self) {
    auto self = static_cast<log_sink_file *>(v_self);
    std::unique_lock<std::mutex> lock{self->m_lock};
    if (self->m_file == nullptr) {
        return -1;
    }
    std::fclose(self->m_file);
    self->m_file = std::fopen(self->m_filename.string().data(), "wb");
    if (self->m_file == nullptr) {
        return -1;
    }
    self->m_file_size = 0;
    return 0;
}

const logger::sink_interface log_sink_file::sink_if{
    write_all,
    flush_all,
    sync_all,
    clear_all,
};

} // namespace kon