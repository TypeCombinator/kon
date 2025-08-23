// SPDX-FileCopyrightText: 2025 TypeCombinator <typecombinator@foxmail.com>
//
// SPDX-License-Identifier: BSD 3-Clause

#ifndef SCOPE_3404B22B_141A_401E_9871_5B2BFE8B6CA8
#define SCOPE_3404B22B_141A_401E_9871_5B2BFE8B6CA8
#include <type_traits>
#include <utility>
#include <kon/xt/attributes.hpp>

namespace kon {

template <typename EF>
class [[nodiscard]] scope_exit {
   public:
    template <typename EFP>
    explicit constexpr scope_exit(EFP &&ef) noexcept(std::is_nothrow_constructible_v<EF, EFP>)
        requires(!std::is_same_v<std::remove_cvref_t<EFP>, scope_exit>)
             && std::is_constructible_v<EF, EFP>
        : exit_function(std::forward<EFP>(ef)) {
    }

    constexpr scope_exit(scope_exit &&other) noexcept(std::is_nothrow_move_constructible_v<EF>)
        : exit_function(std::move<EF>(other.exit_function)) {
        other.dismiss();
    }

    scope_exit(const scope_exit &) = delete;
    scope_exit &operator=(const scope_exit &) = delete;
    scope_exit &operator=(scope_exit &&) = delete;

    constexpr ~scope_exit() {
        if (engaged) {
            // TODO: Handle exception throwing.
            exit_function();
        }
    }

    constexpr void dismiss() noexcept {
        engaged = false;
    }

    constexpr void rehire() noexcept {
        engaged = true;
    }

   private:
    [[no_unique_address]]
    EF exit_function;
    bool engaged = true;
};

template <typename EF>
scope_exit(EF) -> scope_exit<EF>;

namespace detail {
struct exclusive_t {
    exclusive_t() = default;
    KON_DISALLOW_COPY(exclusive_t);
    KON_DISALLOW_MOVE(exclusive_t);
};
} // namespace detail

// It’s a lightweight scope_guard implementation with minimal memory footprint.
// There's a bug for GCC version < 12, see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=100983
#if defined(__clang__) || not defined(__GNUC__) || (__GNUC__ >= 12)
template <auto EF>
struct lw_scope {
    template <typename... Args>
    struct guard;

    template <typename Arg0>
    struct [[nodiscard]] guard<Arg0> {
        constexpr ~guard() noexcept(noexcept(EF(_0))) {
            EF(_0);
        }

        Arg0 _0;
        KON_ATTR_NO_UNIQUE_ADDRESS detail::exclusive_t exclusive;
    };

    template <typename Arg0, typename Arg1>
    struct [[nodiscard]] guard<Arg0, Arg1> {
        constexpr ~guard() noexcept(noexcept(EF(_0, _1))) {
            EF(_0, _1);
        }

        Arg0 _0;
        Arg1 _1;
        KON_ATTR_NO_UNIQUE_ADDRESS detail::exclusive_t exclusive;
    };

    template <typename Arg0, typename Arg1, typename Arg2>
    struct [[nodiscard]] guard<Arg0, Arg1, Arg2> {
        constexpr ~guard() noexcept(noexcept(EF(_0, _1, _2))) {
            EF(_0, _1, _2);
        }

        Arg0 _0;
        Arg1 _1;
        Arg2 _2;
        KON_ATTR_NO_UNIQUE_ADDRESS detail::exclusive_t exclusive;
    };

    // Supporting an arbitrary number of template parameters is not difficult, but doing so will
    // increase build time. It‘s sufficient for most scenarios, but for more complex cases, please
    // use the scope_exit! Certainly, using std::unique_ptr is also a good choice!

    template <typename... Args>
    guard(Args...) -> guard<Args...>;
};
#endif

} // namespace kon
#endif /* scope.hpp */