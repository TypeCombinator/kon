// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef INERTING_A264C25A_9225_4574_986C_1DADACD9283D
#define INERTING_A264C25A_9225_4574_986C_1DADACD9283D
#include <memory>

namespace kon {
template <typename T>
class inerting {
   public:
    constexpr inerting() noexcept {
    }

    constexpr ~inerting() noexcept {
    }

    // Disallow coping and moving.
    inerting(const inerting &) = delete;
    inerting(inerting &&) = delete;
    inerting &operator=(const inerting &) = delete;
    inerting &operator=(inerting &&) = delete;

    template <typename... Args>
    T &construct(Args &&...args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
        return *std::construct_at(std::addressof(storage), std::forward<Args>(args)...);
    }

    void destruct() noexcept(std::is_nothrow_destructible_v<T>) {
        std::destroy_at(std::addressof(storage));
    }

    T &get() & noexcept {
        return *std::addressof(storage);
    }

    const T &get() const & noexcept {
        return *std::addressof(storage);
    }

    T &&get() && noexcept {
        return static_cast<T &&>(*std::addressof(storage));
    }

    const T &&get() const && noexcept = delete;
   private:
    union {
        T storage;
    };
};
} // namespace kon
#endif /* inerting.hpp */