// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause
#ifndef VLM_RING_7478D110_0F44_430D_B16E_68E14D85F85C
#define VLM_RING_7478D110_0F44_430D_B16E_68E14D85F85C
#include <atomic>
#include <cstring>

namespace kon {
// Notice:
// - SPSC lock-free ring buffer.
// - Messages are aligned with 8 bytes by default, and larger alignment requires manual processing.
// - Thers're some UBs.
class vlm_ring {
   public:
    static constexpr uint32_t turn_around_message_type = 0xFFFFFFFFu;

    struct message_head {
        uint32_t type;
        uint32_t length;
    };

    struct zc_scope {
        struct message_head* head;
        uint8_t* data;
    };

    vlm_ring(std::size_t size)
        : buffer(new uint8_t[message_align(size + sizeof(message_head))])
        , windex(0)
        , rindex(0)
        , buffer_size(size) {
    }

    ~vlm_ring() {
        delete[] buffer;
    }

    [[nodiscard]]
    uint8_t* push_begin(uint32_t msg_length) noexcept {
        std::size_t rest;
        msg_length = message_align(sizeof(message_head) + msg_length);

        std::size_t wi = windex.load(std::memory_order_acquire);
        std::size_t ri = rindex.load(std::memory_order_relaxed);
        // |---r----w------rest-------|
        if (wi >= ri) {
            rest = buffer_size - wi;
            if (rest >= msg_length) {
                return buffer + wi;
            }
            if (ri > msg_length) { // Can't be >=, it's ambiguous!
                (new (buffer + wi) message_head)->type = turn_around_message_type; // Turn around
                return buffer;
            }
            return nullptr;
        }
        // |----w------rest-------r---|
        rest = ri - wi;
        if (rest > msg_length) { // Can't be >=, it's ambiguous!
            return buffer + wi;
        }
        return nullptr;
    }

    [[nodiscard]]
    bool push_begin(zc_scope& zcs, uint32_t msg_length) noexcept {
        auto* msg_buffer = push_begin(msg_length);
        if (msg_buffer == nullptr) {
            return false;
        }
        zcs.head = new (msg_buffer) message_head;
        zcs.data = msg_buffer + sizeof(message_head);
        return true;
    }

    void push_end(const message_head& msg_head) noexcept {
        windex.store(
            (reinterpret_cast<std::size_t>(&msg_head) - reinterpret_cast<std::size_t>(buffer))
                + message_align(sizeof(message_head) + msg_head.length),
            std::memory_order_release);
    }

    void push_end(const zc_scope& zcs) noexcept {
        push_end(*zcs.head);
    }

    // If the data is a nullptr, it's UB.
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

    bool push(uint32_t type) noexcept {
        auto* msg_buffer = push_begin(0);
        if (msg_buffer == nullptr) {
            return false;
        }
        auto* head = new (msg_buffer) message_head;

        head->type = type;
        head->length = 0;

        push_end(*head);
        return true;
    }

    [[nodiscard]]
    uint8_t* pop_begin() const noexcept {
        std::size_t ri = rindex.load(std::memory_order_acquire);
        std::size_t wi = windex.load(std::memory_order_relaxed);
        if (wi == ri) [[unlikely]] {
            return nullptr;
        }
        auto* head = new (buffer + rindex) message_head;
        if (head->type != turn_around_message_type) [[likely]] { // Not turn-around type?
            return buffer + ri;
        }
        return buffer; // Turn around
    }

    [[nodiscard]]
    bool pop_begin(zc_scope& zcs) noexcept {
        auto* msg_buffer = pop_begin();
        if (msg_buffer == nullptr) {
            return false;
        }
        zcs.head = new (msg_buffer) message_head;
        zcs.data = msg_buffer + sizeof(message_head);
        return true;
    }

    void pop_end(const message_head& msg_head) noexcept {
        rindex.store(
            (reinterpret_cast<std::size_t>(&msg_head) - reinterpret_cast<std::size_t>(buffer))
                + message_align(sizeof(message_head) + msg_head.length),
            std::memory_order_release);
    }

    void pop_end(const zc_scope& zcs) noexcept {
        pop_end(*zcs.head);
    }

    // If the data is a nullptr, it's UB.
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
    bool empty(std::memory_order m = std::memory_order_acquire) const noexcept {
        return windex.load(m) == rindex.load(m);
    }

    [[nodiscard]]
    bool capacity() const noexcept {
        return buffer_size;
    }

    [[nodiscard]]
    std::size_t write_index() const noexcept {
        return windex.load(std::memory_order_relaxed);
    }

    [[nodiscard]]
    std::size_t read_index() const noexcept {
        return rindex.load(std::memory_order_relaxed);
    }
   private:
    static uint32_t message_align(uint32_t length) noexcept {
        return (length + 7) & (~7u);
    }

    std::atomic_size_t windex, rindex;
    size_t buffer_size;
    uint8_t* buffer;
};
} // namespace kon

#endif /* vlm_ring.hpp */