// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef SPIN_LOCK_BA9F0A7B_7B4C_471E_9909_219135CAED00
#define SPIN_LOCK_BA9F0A7B_7B4C_471E_9909_219135CAED00
// #include <kon/xt/pause.hpp>
#include <thread>
#include <atomic>

namespace kon {
class spin_lock {
    struct [[nodiscard]] spin_wait {
        spin_wait() noexcept = default;

        inline void wait() noexcept {
            if (count++ < yield_threshold) {
                // rt::pause();
            } else {
                if (count == 0) [[unlikely]] {
                    count = yield_threshold;
                }
                std::this_thread::yield();
            }
        }
       private:
        static constexpr uint32_t yield_threshold = 20;
        uint32_t count{0};
    };
   public:
    void lock() noexcept {
        spin_wait spin;
        auto old_state = state.load(std::memory_order_relaxed);
        do {
            while (old_state) {
                spin.wait();
                old_state = state.load(std::memory_order_relaxed);
            }
        } while (!state.compare_exchange_weak(
            old_state, 1, std::memory_order_acquire, std::memory_order_relaxed));
    }

    void unlock() noexcept {
        state.store(0, std::memory_order_release);
    }
   private:
    std::atomic<unsigned char> state{0};
};

} // namespace kon
#endif // spin_lock.hpp