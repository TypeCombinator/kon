// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef LOG_SINK_CIRCULAR_BUFFER_DC4636C7_6068_47ED_91CB_91413C2DA4C7
#define LOG_SINK_CIRCULAR_BUFFER_DC4636C7_6068_47ED_91CB_91413C2DA4C7
#include <mutex>
#include <kon/log/log_frontend.hpp>

namespace kon {
// Overwritable cicular buffer.
struct log_sink_circular_buffer {
    uint8_t* m_buffer{nullptr};
    std::size_t m_capacity{};
    std::size_t m_offset{};
    std::size_t m_round{};
    std::mutex m_lock;

    static constexpr std::string_view buffer_guard_data{"01234567"};

    int initialize(std::size_t capacity);

    void reset();

    struct tail_space {
        const uint8_t* m_first_part;
        std::size_t m_first_part_size;
        const uint8_t* m_second_part;
        std::size_t m_second_part_size;

        std::size_t read_slice(
            std::size_t offset,
            std::uint8_t* slice,
            std::size_t slice_size,
            bool& is_last) const noexcept;
    };

    std::size_t get_tail(std::size_t size, tail_space& space);

    ~log_sink_circular_buffer() {
        if (m_buffer != nullptr) {
            delete[] (m_buffer - buffer_guard_data.size());
        }
    }

    int validate() noexcept;

    static int write_all(void* self, std::string_view data);
    static int flush_all(void* self);
    static int sync_all(void* self);
    static int clear_all(void* self);

    static const logger::sink_interface sink_if;
};
} // namespace kon

#endif // log_sing_circular_buffer.hpp