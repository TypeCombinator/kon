// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef LOG_SINK_FILE_F841EE79_69D9_493A_B994_CF9707301A90
#define LOG_SINK_FILE_F841EE79_69D9_493A_B994_CF9707301A90
#include <filesystem>
#include <mutex>
#include <kon/log/log_frontend.hpp>

namespace kon {

struct log_sink_file {
    log_sink_file(std::size_t file_size_limit) noexcept
        : m_file_size_limit{file_size_limit}

    {
    }

    ~log_sink_file() {
        if (m_file != nullptr) {
            std::fclose(m_file);
            m_file = nullptr;
        }
    }

    int initialize(const std::filesystem::path& filename, const std::string& mode) noexcept;

    static int write_all(void* self, std::string_view data);
    static int flush_all(void* self);
    static int sync_all(void* self);
    static int clear_all(void* self);

    static const logger::sink_interface sink_if;
   private:
    FILE* m_file{nullptr};
    std::filesystem::path m_filename;
    std::size_t m_file_size{};
    std::size_t m_file_size_limit{};
    std::mutex m_lock;
};

} // namespace kon
#endif // log_sink_file.hpp