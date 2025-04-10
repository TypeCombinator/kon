// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause
#ifndef VLM_QUEUE_7478D110_0F44_430D_B16E_68E14D85F85C
#define VLM_QUEUE_7478D110_0F44_430D_B16E_68E14D85F85C

#include <new>
#include <cstdint>
#include <cstring>

namespace kon {

class vlm_queue {
   public:
    static constexpr uint32_t turn_around_message_type = 0xFFFFFFFFu;

    struct message_head {
        uint32_t type;
        uint32_t length;
    };

    struct message {
        struct message_head* head;
        uint8_t* data;
    };

    vlm_queue(std::size_t size)
        : buffer(new uint8_t[message_align(size + sizeof(message_head))])
        , windex(0)
        , rindex(0)
        , buffer_size(size) {
    }

    ~vlm_queue() {
        delete[] buffer;
    }

    [[nodiscard]]
    uint8_t* push_begin(uint32_t msg_length) noexcept {
        std::size_t rest;
        msg_length = message_align(sizeof(message_head) + msg_length);

        // |---r----w------rest-------|
        if (windex >= rindex) {
            rest = buffer_size - windex;
            if (rest >= msg_length) {
                return buffer + windex;
            }
            if (rindex > msg_length) { // Can't be >=, it's ambiguous!
                (new (buffer + windex) message_head)->type =
                    turn_around_message_type; // Turn around
                return buffer;
            }
            return nullptr;
        }
        // |----w------rest-------r---|
        rest = rindex - windex;
        if (rest > msg_length) { // Can't be >=, it's ambiguous!
            return buffer + windex;
        }
        return nullptr;
    }

    [[nodiscard]]
    bool push_begin(message& msg, uint32_t msg_length) noexcept {
        auto* msg_buffer = push_begin(msg_length);
        if (msg_buffer == nullptr) {
            return false;
        }
        msg.head = new (msg_buffer) message_head;
        msg.data = msg_buffer + sizeof(message_head);
        return true;
    }

    void push_end(const message_head& msg_head) noexcept {
        windex = (reinterpret_cast<std::size_t>(&msg_head) - reinterpret_cast<std::size_t>(buffer))
               + message_align(sizeof(message_head) + msg_head.length);
    }

    void push_end(const message& msg) noexcept {
        push_end(*msg.head);
    }

    bool push(uint32_t type, const uint8_t* data, uint32_t length) noexcept {
        auto* msg_buffer = push_begin(length);
        if (msg_buffer == nullptr) {
            return false;
        }
        auto* head = new (msg_buffer) message_head;

        head->type = type;
        head->length = length;
        std::memcpy(msg_buffer + sizeof(message_head), data, length);

        push_end(*head);
        return true;
    }

    [[nodiscard]]
    uint8_t* pop_begin() const noexcept {
        if (windex == rindex) [[unlikely]] {
            return nullptr;
        }
        auto* head = new (buffer + rindex) message_head;
        if (head->type != turn_around_message_type) [[likely]] { // Not turn-around type?
            return buffer + rindex;
        }
        return buffer; // Turn around
    }

    [[nodiscard]]
    bool pop_begin(message& msg) noexcept {
        auto* msg_buffer = pop_begin();
        if (msg_buffer == nullptr) {
            return false;
        }
        msg.head = new (msg_buffer) message_head;
        msg.data = msg_buffer + sizeof(message_head);
        return true;
    }

    void pop_end(const message_head& msg_head) noexcept {
        rindex = (reinterpret_cast<std::size_t>(&msg_head) - reinterpret_cast<std::size_t>(buffer))
               + message_align(sizeof(message_head) + msg_head.length);
    }

    void pop_end(const message& msg) noexcept {
        pop_end(*msg.head);
    }

    bool pop(message_head& msg_head, uint8_t* data, uint32_t length) noexcept {
        auto* msg_buffer = pop_begin();
        if (msg_buffer == nullptr) {
            return false;
        }
        auto* head = new (msg_buffer) message_head;
        if (head->length > length) {
            return false;
        }
        msg_head = *head;
        std::memcpy(data, msg_buffer + sizeof(message_head), head->length);
        return true;
    }

    [[nodiscard]]
    bool empty() noexcept {
        return windex == rindex;
    }

    [[nodiscard]]
    bool capacity() noexcept {
        return buffer_size;
    }

    [[nodiscard]]
    std::size_t write_index() noexcept {
        return windex;
    }

    [[nodiscard]]
    std::size_t read_index() noexcept {
        return rindex;
    }
   private:
    uint32_t message_align(uint32_t length) noexcept {
        return (length + 7) & (~7u);
    }

    std::size_t windex, rindex;
    size_t buffer_size;
    uint8_t* buffer;
};
} // namespace kon

#endif /* vlm_queue.hpp */