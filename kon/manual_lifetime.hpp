#ifndef MANUAL_LIFETIME_A264C25A_9225_4574_986C_1DADACD9283D
#define MANUAL_LIFETIME_A264C25A_9225_4574_986C_1DADACD9283D
#include <memory>

namespace kon {
template <typename T>
class manual_lifetime {
   public:
    manual_lifetime() noexcept {
    }

    ~manual_lifetime() noexcept {
    }

    // Disallow coping and moving.
    manual_lifetime(const manual_lifetime &) = delete;
    manual_lifetime(manual_lifetime &&) = delete;
    manual_lifetime &operator=(const manual_lifetime &) = delete;
    manual_lifetime &operator=(manual_lifetime &&) = delete;

    template <typename... Args>
    void construct(Args &&...args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
        std::construct_at(std::addressof(storage), std::forward<Args>(args)...);
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
#endif /* manual_lifetime.hpp */