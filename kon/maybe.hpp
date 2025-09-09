// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef MAYBE_A7E32BA7_D0FF_4CC0_8064_B631927CDE30
#define MAYBE_A7E32BA7_D0FF_4CC0_8064_B631927CDE30

#include <limits>
#include <cmath>
#include <utility>
#include <functional>
#include <kon/xt/attributes.hpp>

// Experimental Code (WIP)
namespace kon {

template <typename T, typename = void>
struct nothing {
    using is_default = void;
};

template <typename T>
struct nothing<T, std::enable_if_t<std::is_floating_point_v<T>>> {
    static constexpr void make(T *location) noexcept {
        *location = std::numeric_limits<T>::quiet_NaN();
    }

    static constexpr void reset(T &location) noexcept {
        location = std::numeric_limits<T>::quiet_NaN();
    }

    static constexpr bool has(const T &v) noexcept {
        return std::isnan(v);
    }
};

template <typename T>
struct nothing<T, std::enable_if_t<std::is_pointer_v<T>>> {
    static constexpr auto make(T *location) noexcept {
        return nullptr;
    }

    static constexpr void reset(T &location) noexcept {
        location = nullptr;
    }

    static constexpr bool has(const T &v) noexcept {
        return v == nullptr;
    }
};

struct nullopt_t {
    enum class Tag {
        tag
    };

    explicit constexpr nullopt_t(Tag) noexcept {
    }
};

static constexpr nullopt_t nullopt{nullopt_t::Tag::tag};

template <typename T, typename NT = nothing<T>>
class maybe {
   public:
    using value_type = T;

    static constexpr bool is_default_nothing = requires { typename NT::is_default; };

    constexpr maybe() noexcept
        requires is_default_nothing
        : has{false} {
    }

    constexpr maybe() noexcept(std::is_nothrow_invocable_v<decltype(NT::make), T *>)
        requires(!is_default_nothing)
    {
        NT::make(std::addressof(s.val));
    }

    constexpr maybe(nullopt_t) noexcept
        requires is_default_nothing
        : has{false} {
    }

    constexpr maybe(nullopt_t) noexcept(std::is_nothrow_invocable_v<decltype(NT::make), T *>)
        requires(!is_default_nothing)
    {
        NT::make(std::addressof(s.val));
    }

    template <typename... Args>
    constexpr explicit maybe(std::in_place_t, Args &&...args) noexcept(
        std::is_nothrow_constructible_v<T, Args...>)
        requires std::is_constructible_v<T, Args...>
        : s(std::forward<Args>(args)...) {
        if constexpr (is_default_nothing) {
            has = true;
        }
    }

    template <class U, class... Args>
    constexpr explicit maybe(std::in_place_t, std::initializer_list<U> il, Args &&...args) noexcept(
        std::is_nothrow_constructible_v<T, std::initializer_list<U> &, Args &&...>)
        requires std::is_constructible_v<T, std::initializer_list<U> &, Args &&...>
        : s(il, std::forward<Args>(args)...) {
        if constexpr (is_default_nothing) {
            has = true;
        }
    }

    constexpr maybe(const maybe &other) noexcept(std::is_nothrow_copy_constructible_v<T>)
        requires(std::is_copy_constructible_v<T> && !std::is_trivially_copy_constructible_v<T>)
    {
        if constexpr (is_default_nothing) {
            if (other.has) {
                std::construct_at(std::addressof(s.val), other.s.val);
            }
            has = other.has;
        } else {
            std::construct_at(std::addressof(s.val), other.s.val);
        }
    }

    constexpr maybe(const maybe &) noexcept
        requires(std::is_copy_constructible_v<T> && std::is_trivially_copy_constructible_v<T>)
    = default;

    constexpr maybe(maybe &&other) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires(std::is_move_constructible_v<T> && !std::is_trivially_move_constructible_v<T>)
    {
        if constexpr (is_default_nothing) {
            if (other.has) {
                std::construct_at(std::addressof(s.val), std::move(other.s.val));
            }
            has = other.has;
        } else {
            std::construct_at(std::addressof(s.val), std::move(other.s.val));
        }
    }

    constexpr maybe(maybe &&)
        requires std::is_move_constructible_v<T> && std::is_trivially_move_constructible_v<T>
    = default;

    template <typename U = T>
    constexpr explicit(!std::is_convertible_v<U, T>) maybe(U &&other) //
        noexcept(std::is_nothrow_constructible_v<std::in_place_t, U>)
        requires(
            !std::is_same_v<std::decay_t<U>, maybe<T>>
            && !std::is_same_v<std::decay_t<U>, std::in_place_t>
            && std::is_constructible_v<T, U &&>)
        : maybe(std::in_place, std::forward<U>(other)) {
    }

    constexpr maybe &operator=(nullopt_t) noexcept(std::is_nothrow_destructible_v<T>)
        requires is_default_nothing
    {
        if (has) {
            std::destroy_at(std::addressof(s.val));
        }
        has = false;
        return *this;
    }

    constexpr maybe &
        operator=(nullopt_t) noexcept(std::is_nothrow_invocable_v<decltype(&NT::reset), T &>)
        requires(!is_default_nothing)
    {
        NT::reset(s.val);
        return *this;
    }

    constexpr maybe &operator=(const maybe &other) noexcept(
        std::is_nothrow_destructible_v<T> &&       //
        std::is_nothrow_copy_constructible_v<T> && //
        std::is_nothrow_constructible_v<T>)
        requires(
            is_default_nothing &&              //
            std::is_copy_constructible_v<T> && //
            std::is_copy_assignable_v<T> &&    //
            !std::is_trivially_copy_assignable_v<T>)
    {
        if (!other.has) {
            reset();
        } else if (has) {
            s.val = other.s.val;
        } else {
            std::construct_at(std::addressof(s.val), other);
        }
        return *this;
    }

    constexpr maybe &operator=(const maybe &other) noexcept(
        std::is_nothrow_invocable_v<decltype(&NT::reset), T &> && //
        std::is_nothrow_copy_constructible_v<T> &&                //
        std::is_nothrow_constructible_v<T>)
        requires(
            !is_default_nothing &&          //
            std::is_copy_assignable_v<T> && //
            !std::is_trivially_copy_assignable_v<T>)
    {
        if (!other.has) {
            reset();
        } else {
            s.val = other.s.val;
        }
        return *this;
    }

    constexpr maybe &operator=(const maybe &other)
        requires std::is_trivially_copy_assignable_v<T>
    = default;

    constexpr maybe &operator=(maybe &&other) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires(
            is_default_nothing &&              //
            std::is_move_constructible_v<T> && //
            std::is_move_assignable_v<T> &&    //
            !std::is_trivially_move_assignable_v<T>)
    {
        if (!other.has) {
            reset();
        } else if (has) {
            s.val = std::move(other.s.val);
        } else {
            std::construct_at(std::addressof(s.val), std::move(other.s.val));
        }
        return *this;
    }

    constexpr maybe &operator=(maybe &&other) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires(
            !is_default_nothing &&          //
            std::is_move_assignable_v<T> && //
            !std::is_trivially_move_assignable_v<T>)
    {
        if (!other.has) {
            reset();
        } else {
            s.val = std::move(other.s.val);
        }
        return *this;
    }

    constexpr maybe &operator=(maybe &&)
        requires std::is_trivially_move_assignable_v<T>
    = default;

    constexpr bool has_value() const noexcept {
        if constexpr (is_default_nothing) {
            return has;
        } else {
            return !NT::has(s.val);
        }
    }

    constexpr explicit operator bool() const noexcept {
        return has_value();
    }

    constexpr void reset(void) noexcept(std::is_nothrow_destructible_v<T>)
        requires is_default_nothing
    {
        if (has) {
            std::destroy_at(std::addressof(s.val));
        }
        has = false;
    }

    constexpr void reset(void) noexcept(std::is_nothrow_invocable_v<decltype(&NT::reset), T &>)
        requires(!is_default_nothing)
    {
        NT::reset(s.val);
    }

    constexpr T &get() & {
        return s.val;
    }

    inline constexpr const T &get() const & {
        return s.val;
    }

    constexpr T &&get() && {
        return std::move(s.val);
    }

    template <typename... Args>
    constexpr void emplace(Args &&...args) noexcept(
        std::is_nothrow_destructible_v<T> && std::is_nothrow_constructible_v<T, Args...>) {
        if constexpr (is_default_nothing) {
            if (has) {
                std::destroy_at(std::addressof(s.val));
            }
            has = true;
        } else {
            std::destroy_at(std::addressof(s.val));
        }
        std::construct_at(std::addressof(s.val), std::forward<Args>(args)...);
    }

    template <class F>
    constexpr auto and_then(F &&f) & {
        using U = std::invoke_result_t<F, T &>;
        if (has_value()) {
            return std::invoke(std::forward<F>(f), s.val);
        } else {
            return std::remove_cvref_t<U>();
        }
    }

    template <class F>
    constexpr auto and_then(F &&f) && {
        using U = std::invoke_result_t<F, T &&>;
        if (has_value()) {
            return std::invoke(std::forward<F>(f), std::move(s.val));
        } else {
            return std::remove_cvref_t<U>();
        }
    }

    template <class F>
    constexpr auto and_then(F &&f) const & {
        using U = std::invoke_result_t<F, const T &>;
        if (has_value()) {
            return std::invoke(std::forward<F>(f), s.val);
        } else {
            return std::remove_cvref_t<U>();
        }
    }

    template <class F>
    constexpr auto and_then(F &&f) const && {
        using U = std::invoke_result_t<F, const T &&>;
        if (has_value()) {
            return std::invoke(std::forward<F>(f), std::move(s.val));
        } else {
            return std::remove_cvref_t<U>();
        }
    }

    constexpr ~maybe() noexcept
        requires std::is_trivially_destructible_v<T>
    = default;

    ~maybe() noexcept(std::is_nothrow_destructible_v<T>)
        requires(!std::is_trivially_destructible_v<T>)
    {
        if constexpr (is_default_nothing) {
            if (has) {
                std::destroy_at(std::addressof(s.val));
                // has = false;
            }
        } else {
            std::destroy_at(std::addressof(s.val));
        }
    }

   private:
    union storage {
        T val;
    };

    KON_ATTR_NO_UNIQUE_ADDRESS storage s;

    struct empty { };

    KON_ATTR_NO_UNIQUE_ADDRESS std::conditional_t<is_default_nothing, bool, empty> has;
};

} // namespace kon


#endif /* maybe.hpp */