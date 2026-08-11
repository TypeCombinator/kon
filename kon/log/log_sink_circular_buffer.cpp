// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#include <kon/log/log_sink_circular_buffer.hpp>

namespace kon {

int log_sink_circular_buffer::initialize(std::size_t capacity) {
    auto buffer = new uint8_t[capacity + buffer_guard_data.size() * 2];
    std::memcpy(buffer, buffer_guard_data.data(), buffer_guard_data.size());
    std::memcpy(
        buffer + buffer_guard_data.size() + capacity,
        buffer_guard_data.data(),
        buffer_guard_data.size());
    m_buffer = buffer + buffer_guard_data.size();
    m_capacity = capacity;
    return 0;
}

void log_sink_circular_buffer::reset() {
    std::unique_lock<std::mutex> lock{m_lock};
    m_offset = 0;
    m_round = 0;
}

std::size_t log_sink_circular_buffer::get_tail(std::size_t size, tail_space &space) {
    std::unique_lock<std::mutex> lock{m_lock};
    if (m_round > 0) {
        if (size > m_capacity) {
            size = m_capacity;
        }
        std::size_t remain = m_capacity - m_offset;
        if (size > m_offset) {
            space.first_part = m_buffer + (m_capacity - (size - m_offset));
            space.first_part_size = size - m_offset;
            space.second_part = m_buffer;
            space.second_part_size = m_offset;
        } else {
            space.first_part = m_buffer;
            space.first_part_size = size;
            space.second_part = nullptr;
            space.second_part_size = 0;
        }
    } else {
        if (size > m_offset) {
            size = m_offset;
        }
        // Now, size <= m_offset;
        space.first_part = m_buffer;
        space.first_part_size = size;
        space.second_part = nullptr;
        space.second_part_size = 0;
    }
    return size;
}

int log_sink_circular_buffer::write_all(void *v_self, std::string_view input) {
    auto self = static_cast<log_sink_circular_buffer *>(v_self);
    std::unique_lock<std::mutex> lock{self->m_lock};

    std::size_t buffer_remain = self->m_capacity - self->m_offset;
    std::size_t input_remain = input.size();
    const char *input_data = input.data();

    if (input_remain > self->m_capacity) [[unlikely]] {
        std::size_t round = input.size() / self->m_capacity;
        self->m_round += round;
        input_data += ((round - 1) * self->m_capacity);
        std::memcpy(self->m_buffer + self->m_offset, input_data, buffer_remain);
        std::memcpy(self->m_buffer, input_data + buffer_remain, self->m_offset);
        input_data += self->m_capacity;
        input_remain = input.size() - (round * self->m_capacity);
    }
    // input_remain <= self->m_capacity
    if (input_remain <= buffer_remain) [[likely]] {
        std::memcpy(self->m_buffer + self->m_offset, input_data, input_remain);
        self->m_offset += input_remain;
        if (self->m_offset >= self->m_capacity) {
            self->m_offset = 0;
            self->m_round++;
        }
        return 0;
    }
    // input_remain > buffer_remain
    std::memcpy(self->m_buffer + self->m_offset, input_data, buffer_remain);
    input_data += buffer_remain;
    input_remain -= buffer_remain;
    std::memcpy(self->m_buffer, input_data, input_remain);
    self->m_offset = input_remain;
    self->m_round++;
    return 0;
}

int log_sink_circular_buffer::validate() noexcept {
    std::unique_lock<std::mutex> lock{m_lock};
    if (m_offset >= m_capacity) {
        return -1;
    }
    auto guard = m_buffer - buffer_guard_data.size();
    if (std::memcmp(guard, buffer_guard_data.data(), buffer_guard_data.size()) != 0) {
        return -2;
    }
    if (std::memcmp(m_buffer + m_capacity, buffer_guard_data.data(), buffer_guard_data.size())
        != 0) {
        return -3;
    }
    return 0;
}

int log_sink_circular_buffer::flush_all(void *v_self) {
    // auto self = static_cast<log_sink_cirular_buffer *>(v_self);
    return 0;
}

int log_sink_circular_buffer::sync_all(void *v_self) {
    // auto self = static_cast<log_sink_cirular_buffer *>(v_self);
    return 0;
}

int log_sink_circular_buffer::clear_all(void *v_self) {
    // auto self = static_cast<log_sink_cirular_buffer *>(v_self);
    return 0;
}

const logger::sink_interface log_sink_circular_buffer::sink_if{
    write_all,
    flush_all,
    sync_all,
    clear_all,
};
// const logger::sink_interface &log_sink_circular_buffer::get_interface() noexcept {

//     return sink_if;
// }

} // namespace kon